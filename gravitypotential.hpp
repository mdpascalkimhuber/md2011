#ifndef _GRAVITYPOTENTIAL_HPP
#define _GRAVITYPOTENTIAL_HPP

#include "potential.hpp"

/**
 * @brief Subclass from Potential: Calculate the gravitational force between two particles
 */
class GravityPotential : public Potential {
public:
    /**
     * @brief calculate the gravitational force between the two particles and add it to p
     *
     * @param p particle p
     * @param q particle q
     *
     * @return potential energy
     */
    virtual real force(Particle &p, Particle &q);
};

#endif // _GRAVITYPOTENTIAL_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
