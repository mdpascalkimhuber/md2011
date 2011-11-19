#include "world_lc.hpp"

World_LC::World_LC() : World(), cell_N(0), cell_length(0), cell_r_cut(0)
{
  // empty constructor
} 

// derived read_Parameter: call original read_Parameter and open file
// again
void World_LC::read_Parameter(const std::string &filename) 
{
  read_Parameter(filename); 
  
