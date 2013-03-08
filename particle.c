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
    for (float i = 0; i <= (2 * M_PI) + 0.01; i += 0.01)
    {
        glVertex2f(particle->x + sin(i) * particle->r, particle->y + cos(i) * particle->r);
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
        particles[idx].x = (float) idx * 50.0f;
        particles[idx].y = (float) idx * 50.0f;
        particles[idx].r = (float) idx;
    }
}
