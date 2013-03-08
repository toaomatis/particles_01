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
    const double angle = (2 * M_PI) / 100.0f;
    double angle1=0.0;
    glVertex2d(cos(0.0f), sin(0.0f));
    for (int i = 0; i < 100; i++)
    {
        glVertex2d(cos(angle1),sin(angle1));
        angle1 += angle;
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
        particles[idx].x = (float)idx;
        particles[idx].y = (float)idx;
        particles[idx].z = (float)idx;
    }
}
