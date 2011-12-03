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

  virtual void comp_F(); 
  virtual void update_X(); 
  virtual void update_V(); 
  virtual void update_V_in(Cell &C); 

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
