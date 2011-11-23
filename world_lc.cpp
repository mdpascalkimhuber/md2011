#include "world_lc.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>

World_LC::World_LC() : World(), cell_r_cut(0)
{
  // empty constructor
}; 


// calculate the index of a cell with the coordinates
// this is a recursive function
unsigned World_LC::comp_cell_index(unsigned dim, real pos[DIM]) 
{
  // if dim !=0 make a recursive calling
  if (dim > 0) return (unsigned(pos[DIM-dim]/cell_length[DIM-dim]) + cell_N[DIM-dim]*comp_cell_index(dim-1, pos));
  // break condition for recursion
  return unsigned(pos[DIM-dim]/cell_length[DIM-dim]); 
}; 

// calculate the position of a cell ont the basis of the given index,
// this is a recursive function
unsigned World_LC::comp_cell_pos(unsigned dim, real cell_pos[DIM], unsigned index)
{
  if (dim < DIM-1) {
    // recursive calling
    unsigned var = comp_cell_pos(dim+1, cell_pos, index);
    
    // calculate cell_position using only coordinates, that are
    // already calculated 
    cell_pos[dim] = (var % cell_N[dim])*cell_length[dim]; 
    return (var - cell_pos[dim]/cell_length[dim])/cell_N[dim]; 
  }

  // break condition 
  cell_pos[dim] = (index % cell_N[dim])*cell_length[dim]; 
  return ((index-cell_pos[dim])/cell_length[dim])/cell_N[dim]; 
}; 


// derived read_Parameter: call original read_Parameter and open file
// again
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
  
  // set postion of the get pointer in order to avoid to miss data
  parfile.seekg(std::ios_base::beg); 

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

  // calculating number of cells with length of world and cell_r_cut
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
      cell_length[dim] = world_size[dim]/real(cell_N[dim]); 
    }

  // Adding the right number of cells to world 
  /// calculating total number of cells
  unsigned cell_N_tot = 1; 
  for ( unsigned dim = 0; dim < DIM; dim++)
    {
      cell_N_tot *= cell_N[dim];
    }
  /// resize the cell_vector
  cells.resize(cell_N_tot); 
  
  for ( unsigned index = 0; index < cells.size(); index++)
    {
      cells[index].id = index; 
    }
};


// derived read_Parameters: call the original member function and
// distribute all particles over the cells 
void World_LC::read_Particles(const std::string &filename)
{
  // call original read_Parameter of the basis class World
  World::read_Particles(filename); 
  
  // delete all particles of the particles-vector and put them in the
  // right cells
   
  // helper variable
  unsigned index; 
  
  // helper iterator for particle-vector
  std::vector<Particle>::iterator itparticle = particles.begin(); 
  
  // distribute particles while particles-vector not empty
  while (itparticle != particles.end())
    {
      // calculate the index of the right cell 
      index = comp_cell_index(DIM, itparticle->x); 
      
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
  os << std::endl; 

  // give out the length of cells in every dimension
  for (unsigned dim = 0; dim < DIM; dim++)
    os << "Cell_length[" << dim << "]=" << W.cell_length[dim] << " "; 

  return os;
}
