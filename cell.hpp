#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "particle.hpp"
#include <list>

/** @brief For better calculation the world is devided in a number of
 *  cells. Each cell contains a Particle_vector. 
 */
class Cell 
{
public: 
  /**
   * @brief check if cell is empty
   */
  bool empty(); 
  
  // data structure
  /// The cell contains particles ...
  std::list<Particle> particles; 
  // id of cell
  unsigned id; 
  // position of cell
  int cell_pos[DIM]; 
};

std::ostream& operator << (std::ostream &os, const Cell &);

#endif _CELL_HPP_
