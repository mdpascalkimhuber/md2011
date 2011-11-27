#include "world.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>

World::World() : name("unknown"),t(0),delta_t(0),t_end(0),e_kin(0),e_pot(0),e_tot(0)
{
  // empty constructor
}

void World::read_Parameter(const std::string &filename)
{
  // create input filestream
  std::ifstream parfile(filename.c_str());
  // check if file is open
  if (!parfile.is_open())
    throw std::runtime_error(
			     "read_Parameter(): Can't open file '" + filename + "' for reading."
			     );

  // helper strings
  std::string line, option;
  
  // read file till eof
  while (parfile.good())
    {
      // read line from file
      getline(parfile,line);
      // create a string stream
      std::stringstream strstr;
      // put line into string stream
      strstr << line;
      // read option and value from stringstream
      strstr >> option;
      // check options and read values
      if (option=="delta_t")
	strstr >> delta_t;
      if (option=="t_end")
	strstr >> t_end;
      if (option=="name")
	strstr >> name; 
      
      // read length of the observed world
      if (option=="length")
	{
	  // for each dimension
	  for (unsigned dim = 0; dim < DIM; dim++)
	    {
	      strstr >> world_size[dim];
	    }
	}

      // read lower_borders of the observed world
      if (option=="upper_border")
	{
	  // for each dimension
	  for (unsigned dim = 0; dim < DIM; dim++)
	    {
	      strstr >> option;
	      if ( option == "leaving" )
		borders[dim][0] = leaving;
	      if ( option == "unknown" )
		borders[dim][0] = unknown; 
	      if ( option == "periodic" )
		borders[dim][0] = periodic; 
	    }
	}

      // read upper_border of the observed world
      if (option=="lower_border")
	{
	  // for each dimension
	  for (unsigned dim =0; dim < DIM; dim++)
	    {
	      strstr >> option;
	      if (option == "leaving")
		borders[dim][1] = leaving;
	      if ( option == "unknown" )
		borders[dim][1] = unknown; 
	      if ( option == "periodic" )
		borders[dim][0] = periodic; 
	    }
	}				  
    }

  // close file
  parfile.close();
}

void World::read_Particles(const std::string &filename)
{
  // create input filestream
  std::ifstream parfile(filename.c_str()); 
  // check if file is open
  if (!parfile.is_open()) {
    throw std::runtime_error(
			     "read_Particles(): Can't open file '" + filename + "' for reading."
			     );
  }
  // helper strings
  std::string line, option;
    
  // create new particle
  Particle new_particle;

  // read file till eof
  while (parfile.good()) 
    {
      // read line from file
      getline(parfile, line); 
      // create a string stream
      std::stringstream strstr;
      if (line!="") {
	std::cout << line << std::endl;
      // put line into string stream
      strstr << line; 
      // read id
      strstr >> new_particle.id; 
      // read mass 
      strstr >> new_particle.m; 
      // read coordinates
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  strstr >> new_particle.x[dim];
	}
      // read velocities
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  strstr >> new_particle.v[dim];
	}
      // ad new particle to World
      particles.push_back(new_particle);
      }
    }
  particles_N = particles.size(); 
  // close file
  parfile.close(); 
}

std::ostream& operator << (std::ostream& os, World& W) {
  os << "t=" << W.t << " delta_t=" << W.delta_t << " t_end=" << W.t_end
     << " Number of Particles=" << W.particles.size();
  return os;
}

// vim:set et sts=4 ts=4 sw=4 ai ci cin:
