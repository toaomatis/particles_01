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
#include "helper.h"
#include "mainwindow.h"

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
    glColor4f(particle->color.r, particle->color.g, particle->color.b, particle->color.a);
    for (float i = 0; i <= (2 * M_PI) + 0.01; i += 0.01)
    {
        glVertex2f(particle->x + (sin(i) * particle->r), particle->y + (cos(i) * particle->r));
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
        const float r = get_random_float(10.0f, 25.0f);
        particles[idx].x = get_random_float(0.0f + r, WIN_WIDTH_F - r);
        particles[idx].y = get_random_float(0.0f + r, WIN_HEIGHT_F - r);
        particles[idx].r = r;
        particles[idx].color.r = get_random_float(0.5f, 1.0f);
        particles[idx].color.g = get_random_float(0.5f, 1.0f);
        particles[idx].color.b = get_random_float(0.5f, 1.0f);
        particles[idx].color.a = 1.0f;
        printf("[%d] x %4.2f y %4.2f r %4.2f Cr %4.2f Cg %4.2f Cb %4.2f Ca %4.2f \n", idx, particles[idx].x, particles[idx].y,
                particles[idx].r, particles[idx].color.r, particles[idx].color.g, particles[idx].color.b, particles[idx].color.a);
    }
}
