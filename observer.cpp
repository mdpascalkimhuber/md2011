#include "observer.hpp"
#include <iomanip>

// constructor
Observer::Observer(World &_W) : W(_W), e_kin_sum(0.0), e_pot_sum(0.0), mean_e_kin(0.0), mean_e_pot(0.0)
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
  // compute mean values
  comp_mean_value(); 

  // write statistics into the filestream, seperated with tabulars
  statistics
    << W.t << "\t"
    << W.e_pot << "\t"
    << W.e_kin << "\t"
    << W.e_tot << "\t"
    << mean_e_pot << "\t"
    << mean_e_kin << "\t"
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

// calculate mean values of energy
void Observer::comp_mean_value()
{
  // mean value for the first 100 steps
  if (W.t < 100*W.delta_t)
    {
      // add new values to memory-lists
      e_kin_list.push_front(W.e_kin); 
      e_pot_list.push_front(W.e_pot);

      // calculate sums
      e_kin_sum += W.e_kin; 
      e_pot_sum += W.e_pot; 
      
      // calculate mean values
      mean_e_kin = (W.delta_t/W.t) * e_kin_sum; 
      mean_e_pot = (W.delta_t/W.t) * e_pot_sum; 
    }

  // mean value after 100 steps
  else 
    {
      // update memory-lists
      e_kin_list.push_front(W.e_kin); 
      e_pot_list.push_front(W.e_pot); 
      
      // calculate sums
      e_kin_sum = e_kin_sum + e_kin_list.front() - e_kin_list.back(); 
      e_pot_sum = e_pot_sum + e_pot_list.front() - e_pot_list.back(); 

      // delete last elements in value-lists
      e_kin_list.pop_back(); 
      e_pot_list.pop_back(); 
      
      // calculate new mean values
      mean_e_kin = 1.0/e_kin_list.size() * e_kin_sum; 
      mean_e_pot = 1.0/e_pot_list.size() * e_pot_sum; 
    }
}

