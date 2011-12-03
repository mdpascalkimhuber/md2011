#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "particle.hpp"
#include <vector>

// cell structure for linked cell method
class Cell 
{
public: 
  
  // data structure
  /// The cell contains particles ...
  std::vector<Particle> particles; 
  // id of cell
  unsigned id; 
  // position of cell
  unsigned cell_pos[DIM]; 
};

std::ostream& operator << (std::ostream &os, const Cell &);

#endif _CELL_HPP_
