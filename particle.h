/*
 * particle.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#define MUTEX (0)

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
#if TRACE
struct Particle_Trace
{
    int init;
    float x;
    float y;
};
#endif
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

extern const int NUM_PARTICLES;

struct Particle* particle();
void particle_draw(struct Particle *particle);
int particle_interact(struct Particle *a, struct Particle *b);
int particle_move(struct Particle *a);
#if TRACE
void particle_draw_trace(struct Particle *a);
#endif
#endif /* PARTICLE_H_ */
