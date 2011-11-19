#include "velocityverlet.hpp"

// constructor1
VelocityVerlet::VelocityVerlet(World& _W, Potential& _Pot, Observer& _O) : TimeDiscretization(_W,_Pot,_O)
{
  // empty constructor
}

// constructor2
VelocityVerlet::VelocityVerlet(World& _W, Potential* _Pot, Observer& _O) : TimeDiscretization(_W,(*_Pot),_O)
{
  // empty constructor
}

// makes simulation
void VelocityVerlet::simulate()
{
  // calculate forces for t=0
  comp_F();
    
  // write start values
  O.notify();
    
  // while simulation end time not reached
  while (W.t < W.t_end)
    {
      // make a timestep
      timestep(W.delta_t);
      // notify observer
      O.notify();
    }
}

// update all coordinates, forces, velocities for one timestep
void VelocityVerlet::timestep(real delta_t)
{
  // increase time
  W.t += delta_t;

  // update coordinates of all particles
  update_X();

  // calculate all forces and E_pot on the fly
  comp_F();

  // update velocities of all particles and calculate E_kin on the fly
  update_V();

  // handle borders
  handle_borders(); 

  // calculate E_tot
  W.e_tot = W.e_kin + W.e_pot;
}

// compute all forces for one timestep
void VelocityVerlet::comp_F()
{
  // declare iterators for both particles
  std::vector<Particle>::iterator itparticle1, itparticle2; 

  // initialize first iterator
  itparticle1 = W.particles.begin(); 
  
  // initialize E_pot with zero
  W.e_pot = 0;

  // first loop: calculate the force acting on particle1
  while (itparticle1 != W.particles.end())
    {
      // initialize iterator of particle 2 for while-loop
      itparticle2 = W.particles.begin(); 
      // initialize all forces with 0
      for (unsigned dim = 0; dim < DIM; dim++) {
	itparticle1->F[dim] = 0;
      }
      // calculate force of particle2 acting on particle1
      while (itparticle2 != W.particles.end())
	{
	  if(itparticle1 != itparticle2) // no force by the particle itself
	    {
	      // Add force by particle2 and calculate  E_{pot} = E_{pot} * 1/2*V_{1,2}
	      W.e_pot += 0.5*Pot.force(*itparticle1, *itparticle2);
	    } 
	  // increment iterator of particle 2
	  itparticle2++;
	}
      // increment iterator of particle1
      itparticle1++;
    }
}

// update all velocities for one timestep
void VelocityVerlet::update_V()
{
  // initialize iterator
  std::vector<Particle>::iterator itparticle = W.particles.begin();  
  
  // initialize E_kin with 0
  W.e_kin = 0; 

  // update all velocities and calculate E_kin
  while (itparticle != W.particles.end())
    {
      for(unsigned dim = 0; dim < DIM; dim++)
	{
	  // update velocity of particle by formula $ v = v + \delta_t \frac{F+F_{old}}{2m} $
	  itparticle->v[dim] = itparticle->v[dim] + (W.delta_t * (0.5 / itparticle->m) * (itparticle->F[dim] + itparticle->F_old[dim])); 
	  
	  // update E_{kin} = frac{m}{2} * v^2
	  W.e_kin += 0.5 * itparticle->m * sqr(itparticle->v[dim]);
	}
      // increment iterator for while-loop
      itparticle++;
    }
}

// update all coordinates for one timestep
void VelocityVerlet::update_X()
{
  // initialize iterator
  std::vector<Particle>::iterator itparticle = W.particles.begin(); 

  // update coordinates of all particles
  while (itparticle != W.particles.end())
    {
      // update all dimensions 
      for(unsigned dim = 0; dim < DIM; dim++)
	{
	  // update coordinate of particle by formula $ x = x + \delta_t v + \frac{F \delta_t}{2 m} $
          itparticle->x[dim] = itparticle->x[dim] + W.delta_t * (itparticle->v[dim] + ((0.5 / itparticle->m) * itparticle->F[dim] * W.delta_t));
          
	  // update F_old
          itparticle->F_old[dim] = itparticle->F[dim];
	}
      // increment iterator for while-loop
      itparticle++;
    }
}

// handle borders
void VelocityVerlet::handle_borders()
{
  // initialize particle_iterator
  std::vector<Particle>::iterator itparticle = W.particles.begin(); 
  
  // check all particles 
  while ( itparticle != W.particles.end() )
    {
      // check each dimension of particle
      for ( unsigned dim = 0; dim < DIM; dim++)
	{
	  // check lower_border
	  if ( itparticle->x[dim] < 0.0)
	    {
	      // check border type for actual border
	      switch ( W.borders[dim][0] )
		{ // border_type "unknown"
		case unknown:
		  {
		    std::cout << "Lower border (" 
			      << dim << ")x(" << 1
			      << ") is unknown." << std::endl; 
		    itparticle++; // nothing to do, so go to next particle
		    break; 
		  } // border_type "leaving"
		case leaving:
		  { // kill particle and notify
		    itparticle = W.particles.erase(itparticle); 
		    std::cout << "Particle " << (itparticle-1)->id 
			      << " left this world." << std::endl; 
		    break; 
		  }
		}
	    }
	  // check upper_border
	  else if ( itparticle->x[dim] > W.world_size[dim] )
	    {
	      // check border type for actual border
	      switch ( W.borders[dim][1] )
		{ // border_type "unknown"
		case unknown: 
		  {
		    std::cout << "Upper border ("
			      << dim << ")x(" << 2
			      << ") is unknown." << std::endl; 
		    itparticle++; // nothing to do, so go to next particle
		    break; 
		  } // border_type "leaving"
		case leaving: 
		  { // kill particle and notify
		    itparticle = W.particles.erase(itparticle); 
		    std::cout << "Particle " << (itparticle-1)->id
			      << " left this world." << std::endl;
		    break; 
		  }
		}
	    }
	  else // particle still in world, go to next particle
	    if ( dim == (DIM-1) ) itparticle++; 
	}
    }
}
	  
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
