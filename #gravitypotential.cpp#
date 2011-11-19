#include "gravitypotential.hpp"
#include <math.h>

real GravityPotential::force(Particle &p, Particle &q)
{
    // helper variables
    real distance = 0; 
    
    // calculate distance
    for (unsigned dim = 0; dim < DIM; dim++) {
        distance += sqr(p.x[dim]-q.x[dim]);    
    }
    distance = sqrt(distance);                    
    
    // calculate force acting on p
    for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate force on p by q with formula $ F_p = F_p + m_p \cdot m_q \cdot \frac{x_q - x_p}{|x_q - x_p|^3} $
      p.F[dim] += (p.m*q.m)*(q.x[dim]-p.x[dim])/pow(distance, 3);
    }

    // return Gravity Potential
    return -(p.m*q.m)/distance;
}

// vim:set et sts=4 ts=4 sw=4 ai ci cin:
