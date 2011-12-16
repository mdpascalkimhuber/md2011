#ifndef _LENNARDJONESPOTENTIAL_HPP
#define _LENNARDJONESPOTENTIAL_HPP

#include "potential.hpp"

/**
 * @brief Subclass from Potential: Calculate the
 * Lennard-Jones-Potential and the resulting forces
 */
class LJ_Potential : public Potential {
public:
  /**
   * @brief first constructor
   *
   */
  LJ_Potential(); 

  /**
   * @brief second constructor with argument r_cut
   *
   * @param r_cut
   *
   */
  LJ_Potential(real r_cut); 

  /**
   * @brief calculate the force between the two particles defined by
   * the Lennard-Jones-Potential and add it to p
   *
   * @param p particle p
   * @param q particle q
   *
   * @return potential energy
   */
  virtual real force(Particle &p, Particle &q, real distance);
};

#endif //_LENNARDJONESPOTENTIAL_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
