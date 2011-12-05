#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "particle.hpp"
#include <vector>

// cell structure for linked cell method
class Cell 
{
public: 
<<<<<<< HEAD
  /**
   * @brief check if cell is empty
   */
  bool empty(); 
=======
>>>>>>> edc05b93b2a85412d4cbc4ca18d2cdc545149130
  
  // data structure
  /// The cell contains particles ...
  std::vector<Particle> particles; 
  // id of cell
  unsigned id; 
  // position of cell
<<<<<<< HEAD
  int cell_pos[DIM]; 
=======
  real cell_pos[DIM]; 
>>>>>>> edc05b93b2a85412d4cbc4ca18d2cdc545149130
};

std::ostream& operator << (std::ostream &os, const Cell &);

#endif _CELL_HPP_
