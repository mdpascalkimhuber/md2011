#include <cstdlib>
#include <iostream>
#include <cstdio>

#include "world.hpp"
#include "world_lc.hpp"
#include "gravitypotential.hpp"
#include "ljpotential.hpp"
#include "velocityverlet.hpp"
#include "observer.hpp"
#include "observerxyz.hpp"


using namespace std;
int main(int argc, char *argv[]) {

  // check arguments
  if (argc < 2) {
    std::cerr << "error: missing arguments" << std::endl;
    std::cerr << "usage: " << std::endl
              << "\t" << argv[0] << " parameterfile particledatafile" << std::endl;
    return EXIT_FAILURE;
  }

  // instantiate Potential
  ljpotential Pot;

  // create World
  World_LC W; 

  // read Parameters
  W.read_Parameter(argv[1]);

  // read Particles
  W.read_Particles(argv[2]);
    
  // print World configuration
  std::cout << W << std::endl;

  // print particles 
  std::vector<Particle>::iterator it = W.particles.begin(); 
  while (it != W.particles.end())
    {
      std::cout << *it << std::endl; 
      it++;
    }
    
    // create the Observer
  ObserverXYZ O(W);

  // instanciate timediscretization 
  VelocityVerlet Verlet(W, Pot, O);

  // run the simulation
  Verlet.simulate();

  return EXIT_SUCCESS;
}

// vim:set et sts=2 ts=2 sw=2 ai ci cin:
