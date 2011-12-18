#include "velocityverlet_lc.hpp"
#include "observerxyz_lc.hpp"
#include <math.h>
#include <cstdlib>
#include <ctime>

// new constructor with argument World_LC
VelocityVerlet_LC::VelocityVerlet_LC(World_LC& _W, Potential& _Pot, ObserverXYZ_LC &_O) : TimeDiscretization(_W, _Pot, _O), W_LC(_W), O_LC(_O)
{
  // empty constructor
}


// makes simulation
void VelocityVerlet_LC::simulate()
{
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
  // increase time
  W_LC.t += delta_t; 
  // print time
  std::cout << "Timestep: " <<  W_LC.t << std::endl; 

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
    distance += sqr(p.x[dim]-q.x[dim]);    
  }
  
  // return square root of distance^2
  return sqrt(distance);  
}


// update velocities of all particles in one Cell C
void VelocityVerlet_LC::update_V_in(Cell &C)
{
  // initialize iterator 
  std::list<Particle>::iterator p_cell = C.particles.begin(); 

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

// update positions in all cells
void VelocityVerlet_LC::update_X()
{
  // initialize helper variables

  unsigned c_idx = 0; 
  
  // go through cells and update X!
  for (c_idx = 0; c_idx < W_LC.cells.size(); c_idx++)
    {
      update_X_in(c_idx); 
    }

  std::list<Particle>::iterator it_p = W_LC.particles.begin();
  // resort particles, which changed their cell
  while (it_p != W_LC.particles.end())
    {
      // compute global cell_index of new cell
      c_idx = W_LC.compute_cell_index(it_p->x); 
      
      // push particle in new cell
      W_LC.cells[c_idx].particles.push_back(*it_p); 

      // erase particle from particles-vector and increment iterator
      it_p = W_LC.particles.erase(it_p); 
    }
}


// update postions of particles in one cell
void VelocityVerlet_LC::update_X_in(unsigned c_idx)
{
  // initialize particle iterator
  std::list<Particle>::iterator it_p = W_LC.cells[c_idx].particles.begin(); 
  
  // initialize bool variable for border_handling
  bool is_leaving = false; 

  while (it_p != W_LC.cells[c_idx].particles.end())
    {
      // update position of it_p
      for (unsigned dim = 0; dim < DIM; dim++)
	{
	  // update coordinate of particle by formula $ x = x + /delta_t v + \frac{F \delta_t}{2 m} $
	  it_p->x[dim] = it_p->x[dim] + W_LC.delta_t * (it_p->v[dim] + ((0.5 / it_p->m) * it_p->F[dim] * W_LC.delta_t));

	  // update F_old
	  it_p->F_old[dim] = it_p->F[dim]; 
	}
      // initialize bool variable for border_handling
      is_leaving = false; 
      
      //--------------------------------------------------------------------------------
      // border_handling

      for (unsigned dim = 0; dim < DIM; dim++)
	{
	  // check lower border
	  if ( it_p->x[dim] < 0 )
	    { // check border_type
              std::cout << it_p->id << " RAUS" << std::endl;
	      switch (W_LC.borders[dim][0])
		{
		case unknown: // if borders are unknown...
		  {
		    std::cout << "Border (" << dim << ", 0) in unknown." << std::endl; 
		    break; 
		  }
		case leaving: // set is_leaving=true for later handling
		  {
		    is_leaving = true; 
		    break; 
		  }
		case periodic: // correct position
		  {
		    it_p->x[dim] += W_LC.world_size[dim]; 
		    break; 
		  }
		}
	    }

	  // check upper border
	  if ( it_p->x[dim] > W_LC.world_size[dim])
            { // check border_type
	      std::cout << it_p->id << " RAUS upper" << std::endl;
	      switch (W_LC.borders[dim][1])
		{
		case unknown: // if borders are unknown...
		  {
		    std::cout << "Border (" << dim << ", 1) in unknown." << std::endl; 
		    break; 
		  }
		case leaving: // set is_leaving=true for later handling
		  {
		    is_leaving = true; 
		    break; 
		  }
		case periodic: // correct position
		  {
		    it_p->x[dim] -= W_LC.world_size[dim]; 
		    break; 
		  }
		}
	    }
	}
      
      // if particle is leaving, throw it out
      if (is_leaving)
	{
	  // erase particle from cell and increment iterator
	  it_p = W_LC.cells[c_idx].particles.erase(it_p); 
	  // decrement number of particles 
	  W_LC.particles_N--; 
	}
      
      // if particle is not leaving, put it in the right cell
      else
	{
	  // if particle changes cell, put it in W_LC.particles
	  if (W_LC.compute_cell_index(it_p->x) != c_idx)
	    {
	      // push_back
	      W_LC.particles.push_back(*it_p); 
	      
	      // erase particle and increment iterator
	      it_p = W_LC.cells[c_idx].particles.erase(it_p); 
	    }
	  else 
	    {
	      // increment iterator, if nothing happened
	      it_p++; 
	    }
	}
    }
}


// calculate forces
void VelocityVerlet_LC::comp_F()
{
  // initialize potential energy
  W_LC.e_pot = 0; 

  // calculate force of every cell
  for (unsigned c_idx = 0; c_idx < W_LC.cells.size(); c_idx++)
    {
      comp_F_cell(c_idx); 
    }
}

void VelocityVerlet_LC::comp_F_cell(unsigned c_idx)
{
  if (W_LC.cells[c_idx].empty()) return; 
  else
    {
      // define helper variables 
      int loop_idx[DIM]; 
      int current_cell[DIM]; 
      int other_cell[DIM]; 

      // initialize current_cell
      W_LC.compute_cell_pos(c_idx, current_cell); 

      // initialize forces in current_cell
      std::list<Particle>::iterator it_particle = W_LC.cells[c_idx].particles.begin(); 
      while (it_particle != W_LC.cells[c_idx].particles.end())
	{
	  // for all dimensions
	  for (unsigned dim = 0; dim < DIM; dim++)
	    {
	      it_particle->F[dim] = 0; 
	    }
	  it_particle++; 
	}
	 
      // go over all neighbour-cells with 3 for-loops
      for ( loop_idx[0] = -1; loop_idx[0] <2; loop_idx[0]++) 
	{
	  for ( loop_idx[1] = -1; loop_idx[1] <2; loop_idx[1]++) 
	    {
	      for ( loop_idx[2] = -1; loop_idx[2] <2; loop_idx[2]++)
		{
		  // initialize other cell
		  for (unsigned dim = 0; dim < DIM; dim++)
		    {
		      other_cell[dim] = current_cell[dim] + loop_idx[dim]; 
		    }

		  // special case: compute forces in current_cell
		  if ((loop_idx[0] == 0) && (loop_idx[1] == 0) && (loop_idx[2] == 0))
		    {
		      comp_F_same_cell(c_idx); 
		    }

		  // compute forces for other cells
		  else 
		    {
		      comp_F_other_cell(c_idx, other_cell); 
		    }
		}
	    }
	}
    }
}

void VelocityVerlet_LC::comp_F_same_cell(unsigned const c_idx)
{
  /* --------------------------------------------------------------------------------
     function does not contain border_handling for special case that
     only one cell exists
     --------------------------------------------------------------------------------*/


  // initialize iterators for particles
  std::list<Particle>::iterator it_p = W_LC.cells[c_idx].particles.begin(); 
  std::list<Particle>::iterator it_q = W_LC.cells[c_idx].particles.begin(); 

  // distance for force calculation
  real dist = 0; 

  // calculate forces on all particles it_p
  while (it_p != W_LC.cells[c_idx].particles.end())
    {
      // initialize iterator it_q for every it_p-loop
      it_q = W_LC.cells[c_idx].particles.begin(); 
      
      // calculate force of it_q on it_p
      while (it_q != W_LC.cells[c_idx].particles.end())
	{
	  // no force by it_p on itself
	  if (it_p != it_q)
	    {
	      // calculate distance
	      dist = distance(*it_p, *it_q); 
	      
	      // check if dist is small enough
	      if (dist < Pot.r_cut)
		{ // calculate force
		  W_LC.e_pot += 0.5*Pot.force(*it_p, *it_q, dist); 
		  
		}
	    }
	  // increment iterator 
	  it_q++; 
	}
      // increment iterator
      it_p++; 
    }
}

// calculate force for a neighbour cell
void VelocityVerlet_LC::comp_F_other_cell(unsigned const c_idx, int (&other_cell)[DIM])
{
  // helper variables
  bool neighbour = true; // bool variable for border-handling
  unsigned other_idx; // global index of other cell
  real correct_dist[DIM]; // correction of relative distance 
  real dist; // distance 
  
  // initialize correct_dist 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      correct_dist[dim] = 0; 
    }
  
  // border handling
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      // check lower_border of other_cell
      if ( other_cell[dim] < 0 )
	{
	  // check witch border_type for lower border
	  switch (W_LC.borders[dim][0])
	    {
	    case leaving : // no calculation necessary, if other_cell does not exist
	      {
		neighbour = false; 
		break; 
	      }
	    case periodic : 
	      {
		// correct position of other_cell
		other_cell[dim] = W_LC.cell_N[dim]-1; 
		// correct distance (is later added to distance)
		correct_dist[dim] = W_LC.world_size[dim]; 
		break; 
	      }
	    case unknown : // what to do? 
	      {
		std::cout << "Border (" << dim << ", 0) is unknown." << std::endl; 
		neighbour = false; 
		break; 
	      }
	    }
	}
      // check upper_border of other_cell
      else if ( other_cell[dim] > (W_LC.cell_N[dim]-1))
	{
	  // check witch border_type for upper border
	  switch (W_LC.borders[dim][1])
	    {
	    case leaving : // no calculation necessary, if other_cell does not exist
	      {
		neighbour = false; 
		break; 
	      }
	    case periodic : 
	      {
		// correct position of other_cell
		other_cell[dim] = 0; 
		// correct distance (is later added to distance)
		correct_dist[dim] = -(W_LC.world_size[dim]); 
		break; 
	      }
	    case unknown : // what to do?
	      {
		std:: cout << "Border (" << dim << ", 1) is unknown." << std::endl; 
		neighbour = false; 
		break; 
	      }
	    }
	}
    }
  
  // if other_cell exists, calculate forces
  if (neighbour)
    {
      // calculate global index of other_cell
      // other_idx = W_LC.compute_cell_index(real_other_cell); 
      other_idx = W_LC.compute_global(other_cell); 

      // nothing to do for empty cells
      if (W_LC.cells[other_idx].empty()) 
	{
	  return; 
	}

      else 
	{
	  // initialize iterators for particles
	  std::list<Particle>::iterator it_p = W_LC.cells[c_idx].particles.begin(); 
	  std::list<Particle>::iterator it_q = W_LC.cells[other_idx].particles.begin(); 
	  
	  // helper particle
	  Particle mem_par;  
	  
	  // calculate forces acting on it_p
	  while (it_p != W_LC.cells[c_idx].particles.end())
	    {
	      // initialize second iterator for every while-loop
	      it_q = W_LC.cells[other_idx].particles.begin(); 
	      
	      // second while-loop force on it_p by it_q
	      while (it_q != W_LC.cells[other_idx].particles.end())
		{
		  if (it_q != it_p) // no force by it_p on itself
		    {
		
		      // initialize dist
		      dist = 0; 
		      // ... and initialize mass (for force-calculation)
		      mem_par.m = it_q->m; 
		      
		      // calculate correct distance 
		      for (unsigned dim = 0; dim < DIM; dim++)
			{
			  // make a copy of it_q with corrected coordinates
			  dist += sqr(it_p->x[dim] - (it_q->x[dim] - correct_dist[dim])); 
			  mem_par.x[dim] = it_q->x[dim] - correct_dist[dim]; 
			  mem_par.v[dim] = it_q->v[dim]; 
			  mem_par.F[dim] = it_q->F[dim]; 
			  mem_par.F_old[dim] = it_q->F_old[dim]; 
			}
		      dist = sqrt(dist); 
		      
		      // calculate force with given potential
		      if (dist < Pot.r_cut) // check if dist small enough  
			W_LC.e_pot += 0.5*Pot.force(*it_p, mem_par, dist); 
		    }
		  // increment second iterator
		  it_q++; 
		}
	      // increment first iterator
	      it_p++; 
	    }
	}
    }
}


  
