#ifndef _CELL_HPP
#define _CELL_HPP

#include "particle.hpp"

// cell structure for linked cell method
class Cell 
{
public: 
  /// The cell contains particles ...
  std::vector<Particle> particles; 
}

#endif _CELL_HPP
