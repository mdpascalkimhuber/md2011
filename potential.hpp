#ifndef _POTENTIAL_HPP
#define _POTENTIAL_HPP

#include "particle.hpp"

/**
 * @brief abstract Potential class
 */
class Potential {
public:
  /**
   * @brief constructor for the Potential class
   *
   */
  Potential(); 

  /**
   * @brief constructor 2 for the Potential class
   *
   * @param r_cut
   * 
   */
  Potential(real r_cut); 
   
  /**
   * @brief calculate the force between the two particles and add it to p
   *
   * @param p particle p
   * @param q particl q
   * @param distance distance between particle p and particle q
   *
   * @return potential energy
   */
  virtual real force(Particle &p, Particle &q, real distance) = 0;

  // data structure
  /// r_cut 
  real r_cut; 
};

#endif // _POTENTIAL_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
