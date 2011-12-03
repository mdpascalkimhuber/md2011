#ifndef _VELOCITYVERLET_LC_HPP
#define _VELOCITYVERLET_LC_HPP

#include "timediscretization.hpp"
#include "observerxyz_lc.hpp"

/**
 * @brief Implementation of the VelocityVerlet_LC Algorithm
 */
class VelocityVerlet_LC : public TimeDiscretization {
public:
  /**
   * @brief constructor
   *
   * @param _W linked_cell world configuration
   * @param _Pot potential used for force calculation
   * @param _O Observer of the simulation
   */
  VelocityVerlet_LC(World_LC& _W, Potential& _Pot, ObserverXYZ_LC &_O);

  /**
   * @brief run a single timestep
   *
   * @param delta_t length of the timestep
   */
  virtual void timestep(real delta_t);

  /**
   * @brief run the simulation
   */
  virtual void simulate();

  /**
   * @brief calculate the distance between two particles p and q
   */
  virtual real distance(Particle &p, Particle &q); 

  /**
   * @brief calculates the new velocities of the particles in Cell C
   *
   * @param Cell C
   */
  virtual void update_V_in(Cell &C); 

  /**
   * @brief calculates the new velocity of the particles
   */
  virtual void comp_F(); 

  /**
   * @brief calculate forces for all particles in one specific cell
   *
   * @param cell_index global index of cell
   */
  virtual void comp_F_cell(unsigned cell_index); 

  /** 
   * @brief calculate forces of all particles in a neighbour cell
   *
   * @param cell_index of current cell, and cell_position of neighbour
   * cell
   */
  virtual void comp_F_other_cell(const unsigned cell_index, int (&other_cell)[DIM]); 

  virtual void comp_F_same_cell(const unsigned cell_index); 
  virtual void update_X(); 
  virtual void update_V(); 

protected:
  // data structures inherited from TimeDiscretization
  /// linked_cell world 
  World_LC &W_LC; 
  /// observer for linked_cell world
  ObserverXYZ_LC &O_LC; 
  
private:
  VelocityVerlet_LC(); 
};

#endif //_VELOCITYVERLET_LC_HPP
