#include "cell.hpp"

std::ostream& operator << (std::ostream &os, const Cell &C)
{
  return os << "Number of particles in Cell no. " << C.id << ": " << C.particles.size() << std::endl; 
}

  
