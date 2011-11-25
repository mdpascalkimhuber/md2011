#include "velocityverlet_lc.hpp"

// new constructor with argument World_LC
VelocityVerlet_LC::VelocityVerlet_LC(World_LC& _W, Potential& _Pot, Observer &_O) : TimeDiscretization(_W, _Pot, _O), W_LC(_W)
{
  // empty constructor
}

// calculate all forces affecting the particles in one specific cell
// of the world
//void VelocityVerlet_LC::F_comp_in(Cell& C)
//{
  
//}
