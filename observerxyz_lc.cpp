#include "observerxyz_lc.hpp"
#include <iomanip>

// constructor opens a new xyz_file and initialize W_LC
ObserverXYZ_LC::ObserverXYZ_LC(World_LC &_W) : Observer(_W), W_LC(_W)
{
  // open coordinates file
  std::string coordinates_filename = W.name + ".xyz"; 
  //open file, overwrite existing files, take no prisioners
  coordinates_xyz.open(coordinates_filename.c_str()); 
  // and tell the world
  std::cout << "Openend " << coordinates_filename << " for writing." << std::endl; 
}


// destructor closes xyz_file
ObserverXYZ_LC::~ObserverXYZ_LC() 
{
  if(coordinates_xyz.is_open() )
    coordinates_xyz.close();
}


// write all coordinates of every particle in Cell C in an xyz_file
void ObserverXYZ_LC::output_xyz_in(Cell &C)
{
  // initialize iterator for particle vector
  std::vector<Particle>::iterator p_cell = C.particles.begin(); 
  
  while (p_cell != C.particles.end())
    {
      // write particle type (default H)
      coordinates_xyz << "H"; 

      // write all dimensions separated with tabulators
      for (unsigned dim = 0; dim < DIM; dim++)
	{
	  coordinates_xyz << "\t" << p_cell->x[dim]; 
	}
      coordinates_xyz << std::endl; 
      // increment iterator
      p_cell++; 
    }
}


void ObserverXYZ_LC::notify()
{
  Observer::output_statistics(); 
}
