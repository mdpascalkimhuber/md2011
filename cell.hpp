#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "particle.hpp"

// cell structure for linked cell method
class Cell 
{
public: 

  // data structure
  /// The cell contains particles ...
  std::vector<Particle> particles; 
  /// position of the cell
  unsigned cell_pos[DIM];
  /// number of particles in cell
  unsigned cell_particles; 
};

#endif _CELL_HPP_
