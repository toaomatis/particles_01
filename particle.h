/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define TRACE (1)
#define MUTEX (1)

enum BOUNDS
{
    WRAP, BOUNCE, NONE
};

struct Particle_Color
{
    float r;
    float g;
    float b;
    float a;
};

struct Particle_Trace
{
    int init;
    float x;
    float y;
};

struct Particle
{
    int pid;
    int alive;
    int hit;
    float x;
    float y;
    float r;
    float m;
    float vx;
    float vy;
    struct Particle_Color color;
#if TRACE
    int trace_idx;
    struct Particle_Trace *trace;
#endif
#if MUTEX
    pthread_mutex_t mutex;
#endif
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
extern const int TRACE_LENGTH;

struct Particle* particle();
void particle_draw(struct Particle *particle);
int particle_interact(struct Particle *a, struct Particle *b);
int particle_move(struct Particle *a);
void particle_draw_trace(struct Particle *a);

#endif /* PARTICLE_H_ */
