#include "potential.hpp"
#include <limits>

// first constructor
Potential::Potential() : r_cut(std::numeric_limits<real>::max())
{
  // empty constructor
}

// second constructor
Potential::Potential(real r_c) : r_cut(r_c)
{
  // empty constructor
}
