#include "cell.hpp"
#include <iomanip>

// check if cell is empty
bool Cell::empty()
{
  bool empty = false; 
  if (particles.size() == 0)
    {
      empty = true; 
    }
  return empty; 
}

std::ostream& operator << (std::ostream &os, const Cell &C)
{
  os << "Cell no. " << std::setw(6) << C.id << ": "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      os << "   x_" << dim+1 << " = " << std::setw(3) << C.cell_pos[dim] << " "; 
    }
  return os << "   particles: " << C.particles.size() << std::endl;
}

  
