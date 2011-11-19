#ifndef _LENNARDJONESPOTENTIAL_HPP
#define _LENNARDJONESPOTENTIAL_HPP

#include "potential.hpp"

/**
 * @brief Subclass from Potential: Calculate the
 * Lennard-Jones-Potential and the resulting forces
 */
class ljpotential : public Potential {
public:
  /**
   * @brief calculate the force between the two particles defined by
   * the Lennard-Jones-Potential and add it to p
   *
   * @param p particle p
   * @param q particl q
   *
   * @return potential energy
   */
  virtual real force(Particle &p, Particle &q);

};

#endif _LENNARDJONESPOTENTIAL_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
