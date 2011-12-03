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

void VelocityVerlet_LC::update_X()
{
  // do nothing
}

void VelocityVerlet_LC::comp_F()
{
  // do nothing
}
