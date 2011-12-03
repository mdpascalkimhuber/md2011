#include <iostream>
#include "defines.hpp"
#include "particle.hpp"


std::ostream& operator << (std::ostream &Stream, const Particle& P)
{
  // write mass of particle
  Stream << "m: " << P.m << std::endl;

  // write coordinates of particle
  Stream << "x: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.x[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl; 

  // write velocities of particle
  Stream << "v: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.v[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl;

  // write forces of particle
  Stream << "F: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.F[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl;

  // write old forces of particle
  Stream << "F_old: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.F_old[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl << std::endl;

  return Stream;
};
    
    

