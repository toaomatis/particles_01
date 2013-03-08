/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define NUM_PARTICLES (10)

struct Particle
{
    float x;
    float y;
    float z;
};

struct Particle* particle();
int get_num_particles(void);
void particle_draw(struct Particle *particle);

#endif /* PARTICLE_H_ */
