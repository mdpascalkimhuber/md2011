#include "world_lc.hpp"

World_LC::World_LC() : World(), cell_N(0), cell_length(0), cell_r_cut(0)
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
};
