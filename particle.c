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

#define STEP_SIZE (0.01f)

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
    for (float i = 0; i <= (2 * M_PI) + STEP_SIZE; i += STEP_SIZE)
    {
        glVertex2f(particle->x + (sin(i) * particle->r), particle->y + (cos(i) * particle->r));
    }
    glEnd();
}

int particle_interact(struct Particle *a, struct Particle *b)
{
    if ((a->alive != 1) || (b->alive != 1))
    {
        return -1;
    }
    /* Detect center of interaction */
    double p = b->x - a->x;
    double q = b->y - a->y;
    if (CONST_BOUND == WRAP)
    {
        /* Wrap around, use shortest distance */
        if (p > (WIN_WIDTH_F / 2.0f))
        {
            p -= WIN_WIDTH_F;
        }
        else if (p < (-WIN_WIDTH_F / 2.0f))
        {
            p += WIN_WIDTH_F;
        }
        if (q > (WIN_HEIGHT_F / 2.0f))
        {
            q -= WIN_HEIGHT_F;
        }
        else if (q < (-WIN_HEIGHT_F / 2.0f))
        {
            q += WIN_HEIGHT_F;
        }
    }
    const double h2 = p * p + q * q;
    if (CONST_MASS_GRAVITY != 0 && h2 > 1e-10 && (a->hit != 1) && (b->hit != 1))
    {
        /* When gravity is enabled, "drop" particles to bottom */
        double dv;
        dv = CONST_MASS_GRAVITY * b->m / h2;
        a->vx += dv * p;
        a->vy += dv * q;
        dv = CONST_MASS_GRAVITY * a->m / h2;
        b->vx -= dv * p;
        b->vy -= dv * q;
    }
    return 0;
}

int particle_move(struct Particle *a)
{
    a->x += a->vx;
    a->y += a->vy;
    if (a->hit != 1)
    {
        /* if not hit, exert gravity */
        a->vy += CONST_MASS_GRAVITY;
    }
    return 0;
}

static void init(void)
{
    int idx;
    int size_of = sizeof(struct Particle);
    particles = malloc(NUM_PARTICLES * size_of);
    for (idx = 0; idx < NUM_PARTICLES; idx++)
    {
        const float r = get_random_float(5.0f, 10.0f);
        particles[idx].x = get_random_float(0.0f + r, WIN_WIDTH_F - r);
        particles[idx].y = get_random_float(0.0f + r, WIN_HEIGHT_F - r);
        particles[idx].r = r;
        particles[idx].m = CONST_MASS;
        particles[idx].vx = 0.0f;
        particles[idx].vy = 0.0f;
        particles[idx].color.r = get_random_float(0.5f, 1.0f);
        particles[idx].color.g = get_random_float(0.5f, 1.0f);
        particles[idx].color.b = get_random_float(0.5f, 1.0f);
        particles[idx].color.a = 1.0f;
        particles[idx].alive = 1;
        particles[idx].hit = 0;
        printf("[%d] x %4.2f y %4.2f r %4.2f Cr %4.2f Cg %4.2f Cb %4.2f Ca %4.2f \n", idx, particles[idx].x, particles[idx].y,
                particles[idx].r, particles[idx].color.r, particles[idx].color.g, particles[idx].color.b, particles[idx].color.a);
    }
}
