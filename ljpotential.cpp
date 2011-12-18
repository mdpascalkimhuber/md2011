#include "ljpotential.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

// first constructor (r_cut = infinity)
LJ_Potential::LJ_Potential() : Potential()
{
  // empty constructor
}

// second constructor with argument r_cut
LJ_Potential::LJ_Potential(real r_cut) : Potential(r_cut)
{
  // empty constructor
}

// third constructor with argument r_cut, sigma, epsilon
LJ_Potential::LJ_Potential(real r_cut, real sigma, real epsilon) : Potential(r_cut), sigma(1), epsilon(1)
{
  // empty constructor
}

// calculate the force between p and q with the Lennard-Jones
// Potential 
real LJ_Potential::force(Particle &p, Particle &q, real distance)
{
 
  // helper variables
  real fraction = 0; 

  // calculate fraction (sigma/distance)^6 (so it must be calculated only once
  fraction = pow(sigma/distance, 6); 
    
  // calculate force acting on p
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate force acting on p by q with formula:
      // 24\cdot\varepsilon\frac{1}{r_{ij}^2}\cdot\left(frac{sigma}{r_{ij}}\right)^6
      // \cdot \left( 1 - 2 \cdot \left( frac{sigma}{r_{ij}} \right)^6
      // \right) r_{ij}
      p.F[dim] += (24.0 * epsilon)/(sqr(distance)) * fraction * (1.0-2.0*fraction) * (q.x[dim]-p.x[dim]); 
    }

  // return Potential
  return 4*epsilon*fraction*(fraction-1); 
}


void LJ_Potential::read_Parameter(const std::string &filename) 
{
  // create input filestream to open file again
  std::ifstream parfile(filename.c_str()); 
  // check if file is open
  if (!parfile.is_open()) 
    throw std::runtime_error(
			     "read_Parameter(): Can't open file '" + filename + "' for reading."
			     ); 
  // helper strings
  std::string line, option; 
  
  // read file till eof
    while ( parfile.good())
    {
      // read line from file
      getline(parfile, line); 
      // create a string stream
      std::stringstream strstr; 
      // put line into string stream 
      strstr << line; 
      // read option and value from stringstream
      strstr >> option; 
      // check option and read values
      if (option== "sigma")
	strstr >> sigma; 
      if (option== "epsilon")
	strstr >> epsilon; 
      if (option== "cell_r_cut")
	strstr >> r_cut; 
    }

  // close file
  parfile.close(); 
}
