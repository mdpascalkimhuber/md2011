#ifndef _OBSERVERXYZ_HPP
#define _OBSERVERXYZ_HPP

#include "observer.hpp"

/**
 * @brief Subclass from Observer: an observer for the timediscretization with xyz_output
 */
class ObserverXYZ : public Observer
{
public:
  /* @brief constructor
   *
   * opens and creates the files written during observation 
   *
   * @brief _W
   */
  ObserverXYZ(World &_W); 
  
  /* @brief destructor
   *
   * closes all files especially the xyz_file
   *
   */
  ~ObserverXYZ(); 
  
  /**
   * @brief write the coordinates of the particles in xyz_file
   */
  void output_xyz(); 
  
  // derived notify from class Observer 
  virtual void notify(); 

  // xyz_filestream
  std::ofstream coordinates_xyz;

};

#endif // _OBSERVERXYZ_HPP
