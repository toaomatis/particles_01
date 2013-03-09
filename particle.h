/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

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

extern const enum BOUNDS CONST_BOUND;
extern const float CONST_MASS_GRAVITY;
extern const float CONST_MASS;
extern const float CONST_SPEED;
extern const double CONST_RESTITUTION;
extern const double CONST_VMIN;
extern const int CONST_COLLISION;
extern const float CONST_GRAVITY;
extern const int NUM_PARTICLES;

struct Particle* particle();
void particle_draw(struct Particle *particle);
int particle_interact(struct Particle *a, struct Particle *b);
int particle_move(struct Particle *a);

#endif /* PARTICLE_H_ */
