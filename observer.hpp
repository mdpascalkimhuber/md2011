#ifndef _OBSERVER_HPP
#define _OBSERVER_HPP

#include "world.hpp"
#include "world_lc.hpp"
#include <iostream>
#include <fstream>
#include <deque>

/**
 * @brief an observer for the timediscretization
 */
class Observer {
public:
  /**
   * @brief constructor
   *
   * opens and creates the files written during observation
   *
   * @param _W
   */
  Observer(World& _W);

  /**
   * @brief destructor
   *
   * closes the files written during the obervation
   */
  ~Observer(); 

  /**
   * @brief notify the observer that the world has changed
   */
  virtual void notify();

  /** 
   * @brief output statistics like kinetic, potential and total energy
   */
  void output_statistics();

  /** 
   * @brief output the coordinates of the particles
   */
  virtual void output_coordinates();

  /**
   * @brief compute mean-values for e_kin and e_pot
   */
  virtual void comp_mean_value(); 
  
  /// data structure
  // list for e_kin-values
  std::deque<real>e_kin_list; 
  // list for e_pot-values
  std::deque<real>e_pot_list; 
  // sum of last e_kin-values
  real e_kin_sum; 
  // sum of last e_pot-values
  real e_pot_sum; 
  // mean value of e_kin
  real mean_e_kin; 
  // mean value of e_pot
  real mean_e_pot; 
  

protected:
  /// The world we are observing
  World &W;
  /// Statistics filestream
  std::ofstream statistics;
  /// coordiantes filestream
  std::ofstream coordinates;
  
private:
  /// Disabled Constructor
  Observer();
};

#endif // _OBSERVER_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
