/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

struct Particle
{
    float x;
    float y;
    float z;
};

struct Particle* particle();
int get_num_particles(void);
void draw_particles(void);

#endif /* PARTICLE_H_ */
