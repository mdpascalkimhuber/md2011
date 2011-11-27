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

