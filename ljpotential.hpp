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
   * @brief third constructor with arguments r_cut, sigma, epsilon
   *
   * @param r_cut, sigma, epsilon
   */
  LJ_Potential(real r_cut, real sigma, real epsilon); 

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

  /**
   * @brief read paramters like sigma, epsilon, r_cut
   */
  virtual void read_Parameter(const std::string &filename); 


  /// data structure
  // sigma
  real sigma; 
  // epsilon
  real epsilon; 
};

#endif //_LENNARDJONESPOTENTIAL_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
