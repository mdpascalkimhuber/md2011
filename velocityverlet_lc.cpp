#include "velocityverlet_lc.hpp"
#include <math.h>

// new constructor with argument World_LC
VelocityVerlet_LC::VelocityVerlet_LC(World_LC& _W, Potential& _Pot, Observer &_O) : TimeDiscretization(_W, _Pot, _O), W_LC(_W)
{
  // empty constructor
}


// makes simulation
void VelocityVerlet_LC::simulate()
{
  // calculate forces for t=0
  comp_F(); 
  
  // write start values O.notify(); 
  
  // while simulation end time not reached 
  while (W_LC.t < W_LC.t_end)
    {
      // make a timestep
      timestep(W_LC.delta_t); 
      // notify observer
      O.notify(); 
    }
}


// make one timestep in the simulation
void VelocityVerlet_LC::timestep(real delta_t)
{
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
void VelocityVerlet_LC::comp_F_between(Cell& C, Cell& D)
{
  // declare iterators for both cells
  std::vector<Particle>::iterator p_cell1, p_cell2; 

  // initialize first iterator (Cell C)
  p_cell1 = C.particles.begin(); 

  // first loop: calculate the force acting on p_cell1
  while ( p_cell1 != C.particles.end())
    {
      // initialize second iterator (Cell D)
      p_cell2 = D.particles.begin(); 
      while ( p_cell2 != D.particles.end())
	{
	  if (p_cell1 != p_cell2) // no force by particle itself
	    {
	      // compare distance to r_cut
	      if (distance(*p_cell1, *p_cell2) < Pot.r_cut)
		{
		  // Add force by p_cell2 and calculate $ E_{pot} =
		  // E_{pot} * \frac{1}{2} V_{1,2} $
		  W.e_pot += 0.5*Pot.force(*p_cell1, *p_cell2); 
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
  // helper variable for coordinates of neighbour cell
  real coordinates[DIM]; 
  real x[DIM]; 

  // helper iterator for particle vector in Cell C
  std::vector<Particle>::iterator p_cell = C.particles.begin(); 

  // initialize all Forces of all particles in Cell C
  while ( p_cell != C.particles.end())
    {
      // for all three dimensions
      for ( unsigned dim = 0; dim < DIM; dim++)
	p_cell->F[dim] = 0; 
    }

  // three for loops (for each dimension) to get all neighbour cells
  for (x[0] = -1; x[0] < 2; x[0]++) // dimension 1
    {
      for (x[1] = -1; x[1] < 2; x[1]++) // dimension 2
	{
	  for (x[2] = -1; x[2] < 2; x[2]++) // dimension 3
	    {
	      // three if-conditions to check, if neighbour cells exists
	      if ( (0 <= C.cell_pos[0]+x[0]) && (C.cell_pos[0]+x[0] <= W_LC.world_size[0]-W_LC.cell_length[0])) // dimension 1
		{
		  if ( (0 <= C.cell_pos[1]+x[1]) && (C.cell_pos[1]+x[1] <= W_LC.world_size[1]-W_LC.cell_length[1])) // dimension 2
		    {
		      if ( (0 <= C.cell_pos[2]+x[2]) && (C.cell_pos[2]+x[2] <= W_LC.world_size[2]-W_LC.cell_length[2])) // dimension 3
			{
			  // calculate all coordinates of neighbour
			  // cell and save them in helper variable
			  for (unsigned dim = 0; dim < DIM; dim++)
			    {
			      coordinates[dim] = C.cell_pos[dim]+x[dim]; 
			    }
			  // calculate the forces acting on particles
			  // in Cell C from actual neighbour cell
			  comp_F_between(C, W_LC.cells[W_LC.comp_cell_index(DIM, coordinates)]); 
			}
		    }
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
      comp_F_in(*cell); 
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
  // initialize iterator for cell vector
  std::vector<Cell>::iterator cell = W_LC.cells.begin(); 

  // initialize E_kin with 0
  W_LC.e_kin = 0; 

  while ( cell != W_LC.cells.end())
    {
      update_V_in(*cell); 
      cell++; 
    }
}


// update position of all particles in cell C
void VelocityVerlet_LC::update_X_in(Cell &C) 
{
  // initialize iterator for particles vector
  std::vector<Particle>::iterator p_cell = C.particles.begin();

  while (p_cell != C.particles.end())
    {
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  // update coordinate of particle by formula $ x = x + \delta_t v + \frac{F \delta_t}{2 m} $
          p_cell->x[dim] = p_cell->x[dim] + W_LC.delta_t * (p_cell->v[dim] + ((0.5 / p_cell->m) * p_cell->F[dim] * W_LC.delta_t));
	  
	  // update F_old
	  p_cell->F_old[dim] = p_cell->F[dim]; 

	  // delete p_cell->F and p_cell->v: so that particle can't be
	  // moved again at timestep when found in an other cell
	  p_cell->F[dim] = 0.0; 
	  p_cell->v[dim] = 0.0; 
	}
      
      // til now, new positions have been calculated, next step ist
      // border_handling and cell index managing; this will only work
      // in THREE DIMENSIONS

      // initialize bool variable to handle border_type leaving
      bool is_leaving = false; 
      
      // check if particle is still in world, if not handle with it! 
      for ( unsigned dim = 0; dim < DIM; dim++) // checking al dimensions
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
		  }
		case leaving : // particle left the world (so say a prayer)
		  {
		    is_leaving = true; 
		  }
		case periodic : // particle left the world and joins on
				// the other side
		  {
		    // MUST BE WRITTEN 
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
		  }
		case leaving : // particle left the world (so say a prayer)
		  {
		    is_leaving = true; 
		  }
		case periodic : // particle left the world and joins
				// on the other side
		  {
		    // MUST BE WRITTEN 
		  }
		}
	    }
	}
      if ( is_leaving) // if particle left this world, destroy it!
	{
	  // Hasta la vista! Iterator points on next particle in
	  // vector (if it existis)
	  p_cell = C.particles.erase(p_cell); 
	}
      else 
	{
	  // particle is still in world, so update cell index and put
	  // particle in right cell
	  W_LC.cells[W_LC.comp_cell_index(DIM, p_cell->x)].particles.push_back(*p_cell); 
	}
      
      // increment iterator (if still possible)
      if (p_cell != C.particles.end())
	{
	  p_cell++; 
	}
    }
}

// update all positions of all particles in every cell of entire world
void VelocityVerlet_LC::update_X()
{
  // initialize iterator for cell vector
  std::vector<Cell>::iterator cell = W_LC.cells.begin(); 

  while (cell != W_LC.cells.end())
    {
      // update all coordinates of the actual cell
      update_X_in(*cell); 

      // increment cell iterator for while loop
      cell++; 
    }
}


