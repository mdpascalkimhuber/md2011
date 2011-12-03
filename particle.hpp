#ifndef _PARTICLE_HPP
#define _PARTICLE_HPP

#include "defines.hpp"
#include <iostream>

/**
 * @brief Particle data
 *
 * this class contains the particle data
 */
class Particle {
public:
  /// id
  real id; 
  /// Mass
  real m;
  /// Position
  real x[DIM];
  /// Velocity
  real v[DIM];
  /// Force
  real F[DIM];
  /// Force (previous step)
  real F_old[DIM];
};


std::ostream& operator << (std::ostream &Stream, const Particle &P);

#endif // _PARTICLE_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
