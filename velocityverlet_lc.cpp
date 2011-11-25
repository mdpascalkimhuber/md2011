#include "velocityverlet_lc.hpp"
#include <math.h>

// new constructor with argument World_LC
VelocityVerlet_LC::VelocityVerlet_LC(World_LC& _W, Potential& _Pot, Observer &_O) : TimeDiscretization(_W, _Pot, _O), W_LC(_W)
{
  // empty constructor
}

// calculate all forces affecting the particles in one specific cell
// of the world
//void VelocityVerlet_LC::F_comp_in(Cell& C)


// calculate distance between to particles p and q
real VelocityVerlet_LC::distance(Particle &p, Particle &q)
{
  // initialize variable 
  real distance = 0; 
  
  // calculate distance^2
  for (unsigned dim = 0; dim < DIM; dim++) {
    distance += sqr(q.x[dim]-p.x[dim]);    
  }
  
  // return square root of distance^2
  return sqrt(distance);  
}


// calculate all forces affecting the particles in Cell C with origin
// in Cell D
void VelocityVerlet_LC::comp_F_between(Cell& C, Cell& D)
{
  // declare iterators for both cells
  std::vector<Particle>::iterator p_cell1, p_cell2; 

  // initialize first iterator (Cell C)
  p_cell1 = C.particles.begin(); 

  // first loop: calculate the force acting on p_cell1
  while ( p_cell1 != C.particles.end())
    {
      // initialize second iterator (Cell D)
      p_cell2 = D.particles.begin(); 
      while ( p_cell2 != D.particles.end())
	{
	  if (p_cell1 != p_cell2) // no force by particle itself
	    {
	      // compare distance to r_cut
	      if (distance(*p_cell1, *p_cell2) < Pot.r_cut)
		{
		  // Add force by p_cell2 and calculate $ E_{pot} =
		  // E_{pot} * \frac{1}{2} V_{1,2} $
		  W.e_pot += 0.5*Pot.force(*p_cell1, *p_cell2); 
		}
	    }
	  // increment p_cell2 (inner while-loop)
	  p_cell2++; 
	}
      // increment p_cell1 (outer while-loop)
      p_cell1++; 
    }
}

// calculate forces of particles in neighbour cells of Cell C
void VelocityVerlet_LC::comp_F_in(Cell &C)
{
  // helper variable for coordinates of neighbour cell
  real coordinates[DIM]; 
  real x[DIM]; 

  // helper iterator for particle vector in Cell C
  std::vector<Particle>::iterator p_cell = C.particles.begin(); 

  // initialize all Forces of all particles in Cell C
  while ( p_cell != C.particles.end())
    {
      // for all three dimensions
      for ( unsigned dim = 0; dim < DIM; dim++)
	p_cell->F[dim] = 0; 
    }

  // three for loops (for each dimension) to get all neighbour cells
  for (x[0] = -1; x[0] < 2; x[0]++) // dimension 1
    {
      for (x[1] = -1; x[1] < 2; x[1]++) // dimension 2
	{
	  for (x[2] = -1; x[2] < 2; x[2]++) // dimension 3
	    {
	      // three if-conditions to check, if neighbour cells exists
	      if ( (0 <= C.cell_pos[0]+x[0]) && (C.cell_pos[0]+x[0] <= W_LC.world_size[0]-W_LC.cell_length[0])) // dimension 1
		{
		  if ( (0 <= C.cell_pos[1]+x[1]) && (C.cell_pos[1]+x[1] <= W_LC.world_size[1]-W_LC.cell_length[1])) // dimension 2
		    {
		      if ( (0 <= C.cell_pos[2]+x[2]) && (C.cell_pos[2]+x[2] <= W_LC.world_size[2]-W_LC.cell_length[2])) // dimension 3
			{
			  // calculate all coordinates of neighbour
			  // cell and save them in helper variable
			  for (unsigned dim = 0; dim < DIM; dim++)
			    {
			      coordinates[dim] = C.cell_pos[dim]+x[dim]; 
			    }
			  // calculate the forces acting on particles
			  // in Cell C from actual neighbour cell
			  comp_F_between(C, W_LC.cells[W_LC.comp_cell_index(DIM, coordinates)]); 
			}
		    }
		}
	    }
	}
    }
}

