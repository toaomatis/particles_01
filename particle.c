/*
 * particle.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "particle.h"

static void init(void);

static struct Particle *particles;

struct Particle* particle()
{
    init();
    return particles;
}

void particle_draw(struct Particle *particle)
{
    glBegin(GL_LINE_LOOP);
    //glVertex2f(particle->x, particle->y);
    const float r = 0.1;
    for (float i = 0; i <= (2 * M_PI) + 0.01; i += 0.01)
    {
        glVertex2f(particle->x + sin(i) * r, particle->y + cos(i) * r);
    }
    glEnd();
}

int get_num_particles()
{
    return NUM_PARTICLES;
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
        particles[idx].x = (float) idx / (float) NUM_PARTICLES;
        particles[idx].y = (float) idx / (float) NUM_PARTICLES;
        particles[idx].z = (float) idx / (float) NUM_PARTICLES;
    }
}
