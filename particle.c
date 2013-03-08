/*
 * particle.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */
#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

#define NUM_PARTICLES (10)

static void init(void);

static struct Particle *particles;

struct Particle* particle()
{
    init();
    return particles;
}

static void init(void)
{
    int idx;
    int size_of = sizeof(struct Particle);
    particles = malloc(NUM_PARTICLES * size_of);
    for (idx = 0; idx < NUM_PARTICLES; idx++)
    {
#if 0
        particles[idx].x = 0.0f;
        particles[idx].y = 0.0f;
        particles[idx].z = 0.0f;
#endif
        particles[idx].x = (float)idx;
        particles[idx].y = (float)idx;
        particles[idx].z = (float)idx;
    }
}
