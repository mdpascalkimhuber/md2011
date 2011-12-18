#include "observer.hpp"
#include <iomanip>

// constructor
Observer::Observer(World &_W) : W(_W)
{
  // open statistics file
  std::string statistics_filename = W.name + ".statistics";
  // open file, overwrite existing files, take no prisioners
  statistics.open(statistics_filename.c_str());
  // and tell the world
  std::cout << "Opened " << statistics_filename << " for writing." << std::endl;

  // open coordinates file
  std::string coordinates_filename = W.name + ".coordinates";
  // open file, overwrite existing files, take no prisioners
  coordinates.open(coordinates_filename.c_str());
  // and tell the world
  std::cout << "Opened " << coordinates_filename << " for writing." << std::endl;
}

// destructor
Observer::~Observer()
{
  std::cout << "~Observer" << std::endl; 
  // close the statistics file
  if ( statistics.is_open() )
    statistics.close();
  // close coordinates file
  if ( coordinates.is_open() )
    coordinates.close();

}

void Observer::output_statistics()
{
  // write statistics into the filestream, seperated with tabulars
  statistics
    << W.t << "\t"
    << W.e_pot << "\t"
    << W.e_kin << "\t"
    << W.e_tot << "\t"
    << std::endl;
}

void Observer::output_coordinates()
{
  // initialize iterator
  std::list<Particle>::iterator itparticle = W.particles.begin(); 

  // write time
  coordinates << std::setw(15) << W.t;

  // write coordinates of all particles
  while (itparticle != W.particles.end())
    {
      // write all dimensions
      for (unsigned dim = 0; dim < DIM; dim++)
	{
	  coordinates << std::setw(15) << itparticle->x[dim];
	}
      // increment iterator
      itparticle++; 
    }
  // new line
  coordinates << std::endl; 
}


void Observer::notify()
{
  // call output functions
  output_statistics();
  output_coordinates();
}

// vim:set et sts=4 ts=4 sw=4 ai ci cin:
