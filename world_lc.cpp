#include "world_lc.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>

World_LC::World_LC() : World(), cell_r_cut(0)
{
  // empty constructor
}; 

// derived read_Parameter: call original read_Parameter and open file
// again
void World_LC::read_Parameter(const std::string &filename) 
{
  // call original member function to read basic parameters
  read_Parameter(filename); 
  
  // create input filestream to open file again
  std::ifstream parfile(filename.c_str()); 
  // check if file is open
  if (!parfile.is_open()) 
    throw std::runtime_error(
			     "read_Parameter(): Can't open file '" + filename + "' for reading."
			     ); 
  // helper strings
  std::string line, option; 
  
  // set postion of the get pointer in order to avoid to miss data
  parfile.seekg(ios_base::beg); 

  // read file till eof
    while ( parfile.good())
    {
      // read line from file
      getline(parfile, line); 
      // create a string stream
      std::stringstream strstr; 
      // put line into string stream 
      strstr << line; 
      // read option and value from stringstream
      strstr >> option; 
      // check option and read values
      if (option=="cell_r_cut")
	strstr >> cell_r_cut; 
    }

  // close file
  parfile.close(); 

  // calculating number of cells with length of world and cell_c_cut
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate number of cells: the typecast allows a cell
      // length inferior to cell_r_cut
      cell_N[dim] = int(world_size[dim]/cell_r_cut); 
    }
  
  // calculate length of cells with cell_N
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      // l^{cell}_i = \frac{l^{world}_i}{n^{cell}_i}
      cell_length[dim] = double(world_size[dim]/cell_N);

    }

  // Adding the right number of cells to world 
  for (unsigned x1 = 0; x1 < cell_
  };
