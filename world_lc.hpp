#ifndef _WORLD_LC_HPP
#define _WORLD_LC_HPP

#include "world.hpp"


/**
 * @brief Subclass from World: using the linked cell structure in
 * order to reduce costs for calculating forces 
 */

class World_LC : public World {
public: 
  World_LC(); 

  /// additional data structure for linked cell
  // cells
  std::vector<Cell> cells; 
  // Number of cells in every dimension
  int cell_N[DIM];
  // length of cells in every dimension
  real cell_length[DIM];
  // r_cut used for calculation of the cell length
  real cell_r_cut; 
}

#endif _WORLD_LC_HPP
