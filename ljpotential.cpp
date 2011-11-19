#include "ljpotential.hpp"
#include <math.h>

real ljpotential::force(Particle &p, Particle &q)
{
  // free parameters \sigma end \varepsilon
  real sigma = 1; 
  real epsilon = 1; 

  // helper variables
  real distance = 0;
  real fraction = 0; 
    
  // calculate distance
  for (unsigned dim = 0; dim < DIM; dim++) {
    distance += sqr(q.x[dim]-p.x[dim]);    
  }
  distance = sqrt(distance);  

  // calculate fraction (sigma/distance)^6 (so it must be calculated
  // only once
  fraction = pow(sigma/distance, 6); 
    
  // calculate force acting on p
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate force acting on p by q with formula:
      // 24\cdot\varepsilon\frac{1}{r_{ij}^2}\cdot\left(frac{sigma}{r_{ij}}\right)^6
      // \cdot \left( 1 - 2 \cdot \left( frac{sigma}{r_{ij}} \right)^6
      // \right) r_{ij}
      p.F[dim] += (24.*epsilon)/(sqr(distance))*fraction*(1.0-2.*fraction)*(q.x[dim]-p.x[dim]); 
    }

  // return Gravity Potential
  return 4*epsilon*fraction*(fraction-1); 
}

// vim:set et sts=4 ts=4 sw=4 ai ci cin:
