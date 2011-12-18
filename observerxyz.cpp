#include "observerxyz.hpp"
#include <iomanip>

// constructor opens a new xyz_file
ObserverXYZ::ObserverXYZ(World &_W) : Observer(_W)
{
  // open coordinates file
  std::string coordinates_filename = W.name + ".xyz"; 
  //open file, overwrite existing files, take no prisioners
  coordinates_xyz.open(coordinates_filename.c_str()); 
  // and tell the world
  std::cout << "Openend " << coordinates_filename << " for writing." << std::endl; 
}

// destructor closes xyz_file
ObserverXYZ::~ObserverXYZ() 
{
  if(coordinates_xyz.is_open() )
    coordinates_xyz.close();
}

// write coordinates in xyz_file
void ObserverXYZ::output_xyz()
{
  // initialize iterator
  std::list<Particle>::iterator itparticle = W.particles.begin(); 
  
  // write number of particles
  coordinates_xyz << W.particles.size() << std::endl; 

  // write time 
  coordinates_xyz << std::fixed << std::setprecision(8) << "Zeit: " << W.t << std::endl; 

  // write coordinates of all particles
  while (itparticle != W.particles.end())
    {
      // write particle type (default H)
      coordinates_xyz << "H"; 

      // write all dimensions separated with tabulators
      for (unsigned dim = 0; dim < DIM; dim++)
	{
	  coordinates_xyz << "\t" << itparticle->x[dim]; 
	}
      coordinates_xyz << std::endl; 
      // increment iterator
      itparticle++; 
    }
}
  
void ObserverXYZ::notify()
{
  // call output functions for writing
  Observer::notify(); 
  output_xyz(); 
}
