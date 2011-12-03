#include "world_lc.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>


World_LC::World_LC() : World(), cell_r_cut(0), particles_N(0), global_cell_N(1)
{
  // empty constructor
}; 


// calculate the index of a cell with given coordinates pos[DIM]
unsigned World_LC::compute_cell_index(const real (&pos)[DIM]) 
{
  // helper variables
  unsigned index = 0;
  
  // calculate cell_index by formula $ j = j_3 + N_3(j_2 + N_2j_1) $ 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      index *= cell_N[dim]; 
      index += unsigned(pos[dim]/cell_length[dim]);  
    }

  // return value
  return index; 
}; 


// calculate the position of a cell (index_vector) with the global index
void World_LC::compute_cell_pos(unsigned index, unsigned (&cell_pos)[DIM])
{
  // calculate global cell index and save it in cell_pos (cell_pos
  // is the index_vector of a cell)
  for (unsigned dim = DIM; dim > 0; dim--)
    {
      cell_pos[dim-1] = index % cell_N[dim-1]; 
      index = (index - cell_pos[dim-1])/cell_N[dim-1]; 
    }
}; 


// derived read_Parameter: call original read_Parameter and open file
// again to read additional parameters
void World_LC::read_Parameter(const std::string &filename) 
{
  // call original member function to read basic parameters
  World::read_Parameter(filename); 
  
  // create input filestream to open file again
  std::ifstream parfile(filename.c_str()); 
  // check if file is open
  if (!parfile.is_open()) 
    throw std::runtime_error(
			     "read_Parameter(): Can't open file '" + filename + "' for reading."
			     ); 
  // helper strings
  std::string line, option; 
  
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

  // calculating number of cells and length of cells
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate number of cells: the typecast allows a cell
      // length inferior to cell_r_cut
      cell_N[dim] = int(world_size[dim]/cell_r_cut); 
      
      // calculate length of cell (implicit typecast of cell_N)
      cell_length[dim] = world_size[dim]/cell_N[dim]; 

      // calculating total number of cells
      global_cell_N *= cell_N[dim]; 
    }
  
  // Adding the right number of cells to world and writing some
  // information in it
  Cell new_cell; 

  for (unsigned c_idx = 0; c_idx < global_cell_N; c_idx++)
    {
      // give an id to cell
      new_cell.id = c_idx + 1; 

      // give cell_index_pos to cell
      compute_cell_pos(c_idx, new_cell.cell_pos); 
      
      // add cell to cell_vector in world
      cells.push_back(new_cell); 
    }
  
};


// derived read_Parameters: call the original member function and
// distribute all particles over the cells 
void World_LC::read_Particles(const std::string &filename)
{
  // call original read_Parameter of the basis class World
  World::read_Particles(filename); 
  
    // helper variable
  unsigned index; 
    // helper iterator for particle-vector
  std::vector<Particle>::iterator itparticle = particles.begin(); 
  
  // delete particles of particles-vector and put them in right cells
  while (itparticle != particles.end())
    {
      // calculate the index of the right cell 
      index = compute_cell_index(itparticle->x); 

      // add particle to particles-vector in the right cell
      cells[index].particles.push_back(particles.front()); 

      // delete particle in world_particles-vector
      itparticle = particles.erase(itparticle); 
    }
}


std::ostream& operator << (std::ostream& os, World_LC& W) {
  os << "t=" << W.t << " delta_t=" << W.delta_t << " t_end=" << W.t_end
     << " Number of Particles=" << W.particles.size()
     << " cell_r_cut=" << W.cell_r_cut
     << " Number of cells=" << W.cells.size() << std::endl; 
  
  // give out number of cells in every dimension
  for (unsigned dim = 0; dim < DIM; dim++)
    os << "Cell_N[" << dim << "]=" << W.cell_N[dim] << " "; 
  return os << std::endl; 
};
