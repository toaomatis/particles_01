/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define NUM_PARTICLES (100)

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

struct Particle
{
    float x;
    float y;
    float r;
    struct Color color;
};

struct Particle* particle();
int get_num_particles(void);
void particle_draw(struct Particle *particle);

#endif /* PARTICLE_H_ */
