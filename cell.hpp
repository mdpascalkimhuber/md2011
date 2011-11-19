#ifndef _CELL_HPP
#define _CELL_HPP

#include "particle.hpp"

// cell structure for linked cell method
class Cell 
{
public: 

  // data structure
  /// The cell contains particles ...
  std::vector<Particle> particles; 
  /// position of the cell
  unsigned cell_pos;
  /// size of the cell
  real cell_size[DIM];
  /// number of particles in cell
  unsigned cell_particles; 
}

#endif _CELL_HPP
