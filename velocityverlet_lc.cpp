#include "velocityverlet_lc.hpp"
#include "observerxyz_lc.hpp"
#include <math.h>

// new constructor with argument World_LC
VelocityVerlet_LC::VelocityVerlet_LC(World_LC& _W, Potential& _Pot, ObserverXYZ_LC &_O) : TimeDiscretization(_W, _Pot, _O), W_LC(_W), O_LC(_O)
{
  // empty constructor
}


// makes simulation
void VelocityVerlet_LC::simulate()
{
  // For debugging purposes.
  // std::cout << "start simulation" << std::endl; 

  // calculate forces for t=0
  comp_F(); 
  
  // write start values 
  O_LC.notify(); 
  
  // while simulation end time not reached 
  while (W_LC.t < W_LC.t_end)
    {
      // make a timestep
      timestep(W_LC.delta_t); 
      // notify observer
      O_LC.notify(); 
    }
}


// make one timestep in the simulation
void VelocityVerlet_LC::timestep(real delta_t)
{
  // For debugging purposes:
  // std::cout << "Making a timestep" << std::endl; 

  // increase time
  W_LC.t += delta_t; 

  // update coordinates of all particles
  update_X(); 
  // calculate all forces and E_pot on the fly
  comp_F(); 
  
  // update velocities of all particles and calculate E_kin on the fly
  update_V(); 

  // calculate E_tot
  W_LC.e_tot = W_LC.e_kin + W_LC.e_pot; 
}


// calculate distance between to particles p and q
real VelocityVerlet_LC::distance(Particle &p, Particle &q)
{
  // initialize variable 
  real distance = 0; 
  
  // calculate distance^2
  for (unsigned dim = 0; dim < DIM; dim++) {
    distance += sqr(q.x[dim]-p.x[dim]);    
  }
  
  // return square root of distance^2
  return sqrt(distance);  
}


// calculate all forces affecting the particles in Cell C with origin
// in Cell D
void VelocityVerlet_LC::comp_F_between(Cell& C, Cell& D, real cell_distance[DIM])
{
  // declare iterators for both cells
  std::vector<Particle>::iterator p_cell1, p_cell2; 

  // initialize first iterator (Cell C)
  p_cell1 = C.particles.begin(); 

  // initialize distance with zero real dist = 0;
  real dist = 0;

  // first loop: calculate the force acting on p_cell1
  while ( p_cell1 != C.particles.end())
    {
      // initialize second iterator (Cell D)
      p_cell2 = D.particles.begin(); 
      while ( p_cell2 != D.particles.end())
	{
	  if (p_cell1 != p_cell2) // no force by particle itself
	    {

	      // calculating distance
	      for (unsigned dim = 0; dim < DIM; dim++) {
		dist += sqr((p_cell2->x[dim]+cell_distance[dim])-(p_cell1->x[dim]+cell_distance[dim]));    
	      }
	      dist = sqrt(dist); 

	      // compare distance to r_cut
	      if (dist < Pot.r_cut)
		{
		  // Add force by p_cell2 and calculate $ E_{pot} =
		  // E_{pot} * \frac{1}{2} V_{1,2} $
		  W.e_pot += 0.5*Pot.force(*p_cell1, *p_cell2, dist); 
		}
	    }
	  // increment p_cell2 (inner while-loop)
	  p_cell2++; 
	}
      // increment p_cell1 (outer while-loop)
      p_cell1++; 
    }
}

// calculate forces of particles in neighbour cells of Cell C
void VelocityVerlet_LC::comp_F_in(Cell &C)
{
  // helper bool variable
  bool neighbour; 
  // helper variable for coordinates of neighbour cell
  real coordinates[DIM];  // coordinates of neighbour cell 
  real x[DIM]; // just an index for for-loops
  real cell_distance[DIM]; // relative distances for force calculation
 
  // helper iterator for particle vector in Cell C
  std::vector<Particle>::iterator p_cell = C.particles.begin(); 

  // initialize all Forces of all particles in Cell C
  while ( p_cell != C.particles.end())
    {
      // for all three dimensions
      for ( unsigned dim = 0; dim < DIM; dim++)
	p_cell->F[dim] = 0; 
      p_cell++;
    }

  // set bool variable positive (we are optimistic!)
  neighbour = true; 
  // three for loops (for each dimension) to get all neighbour cells
  for (x[0] = -1; x[0] < 2; x[0]++) // dimension 1
    {
      for (x[1] = -1; x[1] < 2; x[1]++) // dimension 2
	{
	  for (x[2] = -1; x[2] < 2; x[2]++) // dimension 3
	    {
	      // calculate all coordinates of neighbour
	      // cell and save them in helper variable
	      for (unsigned dim = 0; dim < DIM; dim++)
		{
		  coordinates[dim] = C.cell_pos[dim]+x[dim]; 
		}
	      
	      // initialize cell_distances
	      for ( unsigned dim = 0; dim < DIM; dim++)
		{
		  cell_distance[dim] = 0; 
		}

	      // six if-conditions to check, if neighbour cells exists
	      // dimension 1 lower border
	      for (unsigned dim = 0; dim < DIM; dim++)
		{

		  // @brief  look for neighbours at each side of your
		  // cell: left/right, up/down, for/behind
		  if ( 0 > (C.cell_pos[dim]+x[dim]*W_LC.cell_length[dim])) 
		    {
		      // BORDER HANDLING
		      // now look at  lower border
		      switch (W_LC.borders[dim][0])
			{
			  // if border_type = leaving, than we have no neighbour
			case leaving : neighbour = false; 
			  break; 
			  // if border_type = periodic, than look for the
			  // cell at the other end of the world
			case periodic: 
			  {
			    coordinates[dim] = (W_LC.world_size[dim] - W_LC.cell_length[dim]); 
			    cell_distance[dim] = -(W_LC.world_size[dim]); 
			    break; 
			  }
			  // if border_type is unknown, we do not know
			  // what to do!
			case unknown: std::cout << "Help, border is unknown." << std::endl; 
			  break; 
			}
		    }
		  //  upper border
		  if ( (C.cell_pos[dim]+x[0]*W_LC.cell_length[dim]) > (W_LC.world_size[dim]-W_LC.cell_length[dim])) 
		    {
		      // BORDER HANDLING
		      // now look at the first dimension, the upper border
		      switch (W_LC.borders[dim][1])
			{
			  // if border_type is leaving, we have no neighbour
			case leaving : neighbour = false; 
			  break; 
			  // if border_type is periodic, look at the cell
			  // at the other end of the world
			case periodic:
			  {
			    coordinates[dim] = 0;  
			    cell_distance[dim] = W_LC.world_size[dim]; 
			  break; 
			  }
			  // if border_type is unknown, we do not know,
			  // what to do.
			case unknown: std::cout << "Help, border is unknown." << std::endl; 
			  break; 
			}
		    }

		}
	      // if the cell we look at is  a neighbour,this means the border_type is in all
	      // dimensions not leaving, (for the moment this means
	      // periodic), we calculate the forces between this cells
	      if ( neighbour )
		{
		  // calculate the forces acting on particles
		  // in Cell C from actual neighbour cell
		  comp_F_between(C, W_LC.cells[W_LC.comp_cell_index(DIM, coordinates)], cell_distance); 
		}
	    }
	}
    }
}


// calculate all forces of all particles: call comp_F_in for all Cells
// in World_LC
void VelocityVerlet_LC::comp_F()
{
  // initialize iterator for cell-vector
  std::vector<Cell>::iterator cell = W_LC.cells.begin(); 
  
  // initialize E_pot with zero
  W_LC.e_pot = 0; 

  // calculate forces for every cell
  while ( cell != W_LC.cells.end())
    {
      // call the function comp_F_in to compute the force
      comp_F_in(*cell); 
      // go to the next cell
      cell++;
    }
}

// update velocities of all particles in one Cell C
void VelocityVerlet_LC::update_V_in(Cell &C)
{
  // initialize iterator 
  std::vector<Particle>::iterator p_cell = C.particles.begin(); 

  // update all velocities and calculate E_kin
  while (p_cell != C.particles.end())
    {
      // for all dimensions
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  // update velocity of particle by formula $ v = v + \delta_t \frac{F+F_{old}}{2m} $
	  p_cell->v[dim] = p_cell->v[dim] + (W_LC.delta_t * (0.5 / p_cell->m) * (p_cell->F[dim] + p_cell->F_old[dim])); 
	  
	  // update E_{kin} = frac{m}{2} * v^2
	  W_LC.e_kin += 0.5 * p_cell->m * sqr(p_cell->v[dim]);
	}
      // increment iterator for while-loop
      p_cell++;
    }
  // std::cout << "update_V_in: updated cell no. " << C.id <<  std::endl; 
}

// update all velocities in every cell C
void VelocityVerlet_LC::update_V()
{
  // For debugging purposes
  // std::cout << "update_V start" << std::endl; 

  // initialize iterator for cell vector
  std::vector<Cell>::iterator cell = W_LC.cells.begin(); 
  
  // initialize E_kin with 0
  W_LC.e_kin = 0; 

  // update the velocity for each cell
  // therefore call the function update_V_in
  while ( cell != W_LC.cells.end())
    {
      update_V_in(*cell); 
      cell++; 
    }
}


// update position of all particles in cell C
void VelocityVerlet_LC::update_X_in(Cell &C) 
{
  // For debugging purposes:
  // std::cout << "update_X_in start with cell no" << C.id << std::endl; 

  // initialize iterator for particles vector
  std::vector<Particle>::iterator p_cell = C.particles.begin();

  // a while loop to look for each cell  
  while (p_cell != C.particles.end())
    {
      // and to look for each dimension
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  // update coordinate of particle by formula $ x = x + \delta_t v + \frac{F \delta_t}{2 m} $
          p_cell->x[dim] = p_cell->x[dim] + W_LC.delta_t * (p_cell->v[dim] + ((0.5 / p_cell->m) * p_cell->F[dim] * W_LC.delta_t));
	  
	  // update F_old
	  p_cell->F_old[dim] = p_cell->F[dim]; 
	}
      // For debugging purposes
      // std::cout << "Cell no. " << C.id << *p_cell << std::endl; 

      // till now, new positions have been calculated, next step is
      // border_handling and cell index managing; this will only work
      // in THREE DIMENSIONS

      // initialize bool variable to handle border_type leaving
      bool is_leaving = false; 
      
      // check if particle is still in world, if not handle with it! 
      for ( unsigned dim = 0; dim < DIM; dim++) // checking all dimensions
	{
	  if ( p_cell->x[dim] < 0) // lower border
	    {
	      // border handling: check which border_type
	      switch (W_LC.borders[dim][0])
		{
		case unknown : // nothing to do MAYBE STOP SIMULATION
		  {
		    std::cout << "Border (" << dim << ", " << 0 
			      << ") is unknown. Don't know what to do!" << std::endl;  
		    break;
		  }
		case leaving : // particle left the world (so say a prayer)
		  {
		    is_leaving = true; 
		    break;
		  }
		case periodic : // particle left the world and joins on
				// the other side
		  {
		    p_cell->x[dim] = W_LC.world_size[dim] + p_cell->x[dim];
		    break; 
		  }
		}
	    }
	  if ( p_cell->x[dim] > W_LC.world_size[dim]) // upper border
	    {
	      // border handling: check which border_type
	      switch (W_LC.borders[dim][1]) 
		{
		case unknown : // nothing to do MAYBE STOP SIMULATION
		  {
		    std::cout << "Border (" << dim << ", " << 0 
			      << ") is unknown. Don't know what to do!" << std::endl;  
		    break;
		  }
		case leaving : // particle left the world (so say a prayer)
		  {
		    is_leaving = true; 
		    break; 
		  }
		case periodic : // particle left the world and joins
				// on the other side
		  {
		    // MUST BE WRITTEN 
		    p_cell->x[dim] = p_cell->x[dim] - W_LC.world_size[dim]; 
		    break; 
		  }
		}
	    }
	}
      if ( is_leaving ) // if particle left this world, destroy it!
	{
	  // For debbuging purposes
	  // std::cout << "Hasta la vista!" << std::endl; 

	  // Hasta la vista! Iterator points on next particle in
	  // vector (if it existis)
	  p_cell = C.particles.erase(p_cell); 
	  W_LC.particles_N--; 
	}
      else if ( !is_leaving) 
	{
	  // particle is still in world, so update cell index and put
	  // particle in particle vector of the World_LC class
	  // if ( C.id != W_LC.comp_cell_index(DIM, p_cell->x))
	  //{
	  // put particle in particles vector of World_LC (memory)
	  W_LC.particles.push_back(*p_cell); 
	  // erase particle from current cell
	  // for debugging
	  // std::cout << "paricle is leaving cell" << std::endl;
	  p_cell = C.particles.erase(p_cell); 
	}
    }
  // increment iterator (if still possible)
  if (p_cell != C.particles.end())
    {
      p_cell++; 
    }
}

// update all positions of all particles in every cell of entire world
void VelocityVerlet_LC::update_X()
{
  // For debugging purposes
  // std::cout << "update X start" << std::endl; 

  // initialize iterator for cell vector
  std::vector<Cell>::iterator cell = W_LC.cells.begin(); 

  while (cell != W_LC.cells.end())
    {
      // update all coordinates of the actual cell
      update_X_in(*cell); 

      // increment cell iterator for while loop
      cell++; 
    }

  // initialize iterator for particle vector
  std::vector<Particle>::iterator p_cell = W_LC.particles.begin(); 

  // put particles, which changed their cell from particle vector
  // (used as memory) into the right cells 
  while (p_cell != W_LC.particles.end())
    {
      // put particle in right cell
      W_LC.cells[W_LC.comp_cell_index(DIM, p_cell->x)].particles.push_back(*p_cell); 

      // erase particle from memory
      p_cell = W_LC.particles.erase(p_cell); 
    }
}




