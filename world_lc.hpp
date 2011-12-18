#ifndef _WORLD_LC_HPP
#define _WORLD_LC_HPP

#include "world.hpp"
#include "cell.hpp"



/**
 * @brief Subclass from World: using the linked cell structure in
 * order to reduce costs for calculating forces 
 */

class World_LC : public World {
public: 
  World_LC(); 

  /**
   * @brief read world parameters from given parameter file
   * (*.parameter). In addition to the original member function,
   * read_parameters now also reads r_cut and calculates all derived
   * data. 
   *
   * @param filename filename of the parameter file
   */
  virtual void read_Parameter(const std::string &filename);

  /**
   * @brief read the particles from given particles file
   * (*.particles).
   * In addition to the original member function, read_particles
   * now distributes the particles over the cells. 
   *
   * @param filename filename of the particles file
   */
  virtual void read_Particles(const std::string &filename); 

  /**
   * @brief calculate the index of the cell on the basis of cartesian
   * coordinates
   */
  virtual unsigned compute_cell_index(const real (&pos)[DIM]);

  /*
   * @brief calculate the position in cartesian coordinates of the
   * given cell on the basis of the index in the cell_vector
   */

  virtual void compute_cell_pos(unsigned index, int (&cell_pos)[DIM]); 

  /*
   * @brief calculate the cell-index on the basis of cell-coordinates
   * of a given cell 
   */
  virtual unsigned compute_global(const int (&cell_pos)[DIM]); 

  /**
   * @brief initialize all particles with Maxwell-Boltzmann
   * distributed velocities
   *
   * @param random_seed
   */
  virtual void maxwell_boltzmann(); 


  /// additional data structure for linked cell
  // cells
  std::vector<Cell> cells; 
  // Number of cells in every dimension
  int cell_N[DIM];
  // length of cells in every dimension
  real cell_length[DIM];
  // r_cut used for calculation of the cell length
  real cell_r_cut; 
  // total number of particles in the world
  unsigned particles_N; 
  // total number of cells in the world 
  unsigned global_cell_N; 

  /// additional data structure for thermostat
  // start temperature
  real set_start_temp; 
  // target temperature
  real therm_target_temp; 
  // thermostat time step intervall
  int therm_step_interval; 
  // random seed for velocity distribution
  unsigned random_seed; 

};

std::ostream& operator << (std::ostream& os, World_LC& W);
#endif _WORLD_LC_HPP
