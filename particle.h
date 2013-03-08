/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define NUM_PARTICLES (100)

enum BOUNDS
{
    WRAP, BOUNCE, NONE
};

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

struct Particle
{
    int alive;
    int hit;
    float x;
    float y;
    float r;
    float m;
    float vx;
    float vy;
    struct Color color;
};

#define CONST_BOUND (WRAP)
#define CONST_MASS_GRAVITY (0.05f)
#define CONST_MASS (1.5f)

struct Particle* particle();
void particle_draw(struct Particle *particle);
int particle_interact(struct Particle *a, struct Particle *b);
int particle_move(struct Particle *a);

#endif /* PARTICLE_H_ */
