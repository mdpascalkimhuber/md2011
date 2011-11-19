#ifndef _WORLD_HPP
#define _WORLD_HPP

#include "defines.hpp"
#include "particle.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/**
 * @brief the world class holds all information of the simulation environment
 */
class World {
public:
  World();

  /**
   * @brief read the world parameters from the given parameter file
   *
   * parameter file example
   * \code
   * delta_t 0.1
   * t_end 1.0
   * \endcode
   *
   * @param filename filename of the parameter file
   */
  void read_Parameter(const std::string &filename);

  /**
   * @brief read the particles from the given data file
   *
   * parameter file example
   * \code
   * 
   * m 1
   * x 0 0 
   * v 0 0 
   * \endcode
   *
   * @param filename filename of the particle data file
   */
  void read_Particles(const std::string &filename);

  // data structures
  /// Name of the simulated world
  std::string name;
  /// Current time
  real t;
  /// Timestep
  real delta_t;
  /// End of simulation
  real t_end;
  /// kinetic energy
  real e_kin;
  /// potential energy
  real e_pot;
  /// total energy
  real e_tot;
  /// Vector of particles
  std::vector<Particle> particles;
  /// size of world (length in all dimensions)
  real world_size[DIM]; 
  /// bordertype as DIM x 2 matrice
  BorderType borders[DIM][2];
}; 

/**
 * @brief a ostream operator for the World class
 *
 * @param os stream object
 * @param W the world
 *
 * @return resulting stream object
 */
std::ostream& operator << (std::ostream& os, World& W);

#endif // _WORLD_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
