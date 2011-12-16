#include "ljpotential.hpp"
#include <math.h>

// first constructor (r_cut = infinity)
LJ_Potential::LJ_Potential() : Potential()
{
  // empty constructor
}

// second constructor with argument r_cut
LJ_Potential::LJ_Potential(real r_cut) : Potential(r_cut)
{
  // empty constructor
}

// calculate the force between p and q with the Lennard-Jones
// Potential 
real LJ_Potential::force(Particle &p, Particle &q, real distance)
{
  // free parameters \sigma end \varepsilon
  real sigma = 1; 
  real epsilon = 1; 

  // helper variables
  real fraction = 0; 

  // calculate fraction (sigma/distance)^6 (so it must be calculated only once
  fraction = pow(sigma/distance, 6); 
    
  // calculate force acting on p
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate force acting on p by q with formula:
      // 24\cdot\varepsilon\frac{1}{r_{ij}^2}\cdot\left(frac{sigma}{r_{ij}}\right)^6
      // \cdot \left( 1 - 2 \cdot \left( frac{sigma}{r_{ij}} \right)^6
      // \right) r_{ij}
      p.F[dim] += (24.0 * epsilon)/(sqr(distance)) * fraction * (1.0-2.0*fraction) * (q.x[dim]-p.x[dim]); 
    }

  // return Potential
  return 4*epsilon*fraction*(fraction-1); 
}


