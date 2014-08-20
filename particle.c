/*
 * particle.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "coll3d.h"
#include "particle.h"
#include "helper.h"
#include "mainwindow.h"

static const float STEP_SIZE = (0.01f);

const enum BOUNDS CONST_BOUND = NONE;
const float CONST_MASS_GRAVITY = 0.05f;
const float CONST_GRAVITY = 0.00f;
const float CONST_MASS = 1.5f;
const float CONST_SPEED = 0.0f;
const double CONST_RESTITUTION = 0.85f;
const double CONST_VMIN = 1E-20;
const int CONST_COLLISION = 1;
const int NUM_PARTICLES = 250;
const int TRACE_LENGTH = 100;

static void init(void);

static struct Particle *particles = NULL;

struct Particle* particle()
{
    if (particles == NULL)
    {
        init();
    }
    return particles;
}

void particle_draw(struct Particle *a)
{
#if MUTEX
    pthread_mutex_lock(&(a->mutex));
#endif
    glColor4f(a->color.r, a->color.g, a->color.b, a->color.a);
#if 0
    glBegin(GL_LINE_LOOP);
    for (float i = 0; i <= (2 * M_PI) + STEP_SIZE; i += STEP_SIZE)
    {
        glVertex2f(a->x + (sin(i) * a->r), a->y + (cos(i) * a->r));
    }
    glEnd();
#else
    glTranslatef(a->x, a->y, a->z);
    glutSolidSphere(a->r, 10, 10);
#endif
#if MUTEX
    pthread_mutex_unlock(&(a->mutex));
#endif
}

int particle_interact(struct Particle *a, struct Particle *b)
{
#if MUTEX
    pthread_mutex_lock(&(a->mutex));
    pthread_mutex_lock(&(b->mutex));
#endif
    int err = -1;
#if 0
    collision3D(a, b, &err);
#else
    if ((a->alive != 1) || (b->alive != 1))
    {
        return -1;
    }
    /* Detect center of interaction */
    double p = b->x - a->x;
    double q = b->y - a->y;
    double r = b->z - a->z;
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
    const double h2 = (p * p) + (q * q) + (r * r);
    if (CONST_COLLISION != 0)
    {
        /* Collisions enabled */
        double h = sqrt(h2);
        if (h < (a->r + b->r))
        {
            /* Collision detected */
            a->hit = b->hit = 1;
#if 0
            if (Constants.MUSH)
            {
                /*
                 * When mush is enabled, merge both particles, where the
                 * heaviest particle survives
                 */
                /*
                 * Detect lightest particle, merge with heaviest and flag
                 * for deletion
                 */
                if (m < b.m)
                {
                    /* Delte this particle */
                    double t = m / (b.m + m);
                    b.x += p * t;
                    b.y += q * t;
                    b.vx += (vx - b.vx) * t;
                    b.vy += (vy - b.vy) * t;
                    b.m += m;
                    b.z = Math.sqrt((b.m / b.d) / Math.PI);
                    delete = true;
                }
                else
                {
                    /* Delete other particle */
                    double t = b.m / (b.m + m);
                    x += p * t;
                    y += q * t;
                    vx += (b.vx - vx) * t;
                    vy += (b.vy - vy) * t;
                    m += b.m;
                    z = Math.sqrt((m / d) / Math.PI);
                    b.delete = true;
                }
                return true;
            }
            else
#endif
            if (h > 1e-10)
            {
                /* Compute the elastic collision of two particles */
                /* http://en.wikipedia.org/wiki/Elastic_collision */
                double v1, v2, r1, r2, s, t, v;
                /* Normalized impact direction */
                p /= h;
                q /= h;
                r /= h;
                /* Impact velocity */
                v1 = a->vx * p + a->vy * q + a->vz * r;
                v2 = b->vx * p + b->vy * q + b->vz * r;
                /* Remainder velocity */
                r1 = a->vx * q - a->vy * p - a->vz * r;
                r2 = b->vx * q - b->vy * p - b->vz * r;
                if (v1 < v2)
                {
#if MUTEX
                    pthread_mutex_unlock(&(b->mutex));
                    pthread_mutex_unlock(&(a->mutex));
#endif

                    return 0;
                }
                /* Combine mass */
                s = a->m + b->m;
                if (s == 0)
                {
#if MUTEX
                    pthread_mutex_unlock(&(b->mutex));
                    pthread_mutex_unlock(&(a->mutex));
#endif
                    return 0;
                }
                t = (v1 * a->m + v2 * b->m) / s;
                v = t + CONST_RESTITUTION * (v2 - v1) * b->m / s;
                a->vx = v * p + r1 * q;
                a->vy = v * q - r1 * p;
                v = t + CONST_RESTITUTION * (v1 - v2) * a->m / s;
                b->vx = v * p + r2 * q;
                b->vy = v * q - r2 * p;
            }
        }
    }
    if (CONST_MASS_GRAVITY != 0 && h2 > 1e-10 && (a->hit != 1) && (b->hit != 1))
    {
        /* When gravity is enabled, "drop" particles to each other */
        double dv;
        dv = CONST_MASS_GRAVITY * b->m / h2;
        a->vx += dv * p;
        a->vy += dv * q;
        a->vz += dv * r;
        dv = CONST_MASS_GRAVITY * a->m / h2;
        b->vx -= dv * p;
        b->vy -= dv * q;
        b->vz -= dv * r;
    }
#endif
#if MUTEX
    pthread_mutex_unlock(&(b->mutex));
    pthread_mutex_unlock(&(a->mutex));
#endif
    return err;
}

int particle_move(struct Particle *a)
{
#if MUTEX
    pthread_mutex_lock(&(a->mutex));
#endif
    a->x += a->vx;
    a->y += a->vy;
    a->z += a->vz;
    if (CONST_BOUND != NONE)
    {
        /* The particles should react to the screen boundary */
        if (a->x + a->r > WIN_WIDTH_F)
        {
            if (CONST_BOUND == WRAP)
            {
                a->x -= WIN_WIDTH_F;
            }
            else if (CONST_BOUND == BOUNCE)
            {
                if (a->vx > 0)
                {
                    a->vx *= CONST_RESTITUTION; /* restitution */
                }
                a->vx = -fabs(a->vx) - CONST_VMIN; /* reverse velocity */
                a->hit = 1;
                /* Check if location is completely off screen */
                if (a->x - a->r > WIN_WIDTH_F)
                {
                    a->x = WIN_WIDTH_F + a->r;
                }
            }
        }
        /* Display x */
        if (a->x - a->r < 0.0f)
        {
            if (CONST_BOUND == WRAP)
            {
                a->x += WIN_WIDTH_F;
            }
            else if (CONST_BOUND == BOUNCE)
            {
                if (a->vx < 0)
                {
                    a->vx *= CONST_RESTITUTION;
                }
                a->vx = fabs(a->vx) + CONST_VMIN;
                a->hit = 1;
                if (a->x + a->r < 0.0f)
                {
                    a->x = -a->r;
                }
            }
        }

        if (a->y + a->r > WIN_HEIGHT_F)
        {
            if (CONST_BOUND == WRAP)
            {
                a->y -= WIN_HEIGHT_F;
            }
            else if (CONST_BOUND == BOUNCE)
            {
                if (a->vy > 0)
                {
                    a->vy *= CONST_RESTITUTION;
                }
                a->vy = -fabs(a->vy) - CONST_VMIN;
                a->hit = 1;
                if (a->y - a->r > WIN_HEIGHT_F)
                {
                    a->y = WIN_HEIGHT_F + a->r;
                }
            }
        }
        /* Display y */
        if (a->y - a->r < 0.0f)
        {
            if (CONST_BOUND == WRAP)
            {
                a->y += WIN_HEIGHT_F;
            }
            else if (CONST_BOUND == BOUNCE)
            {
                if (a->vy < 0)
                {
                    a->vy *= CONST_RESTITUTION;
                }
                a->vy = fabs(a->vy) + CONST_VMIN;
                a->hit = 1;
                if (a->y + a->r < 0)
                {
                    a->y = -a->r;
                }
            }
        }
    }
    if (a->hit != 1)
    {
        /* if not hit, exert gravity */
        a->vy += CONST_GRAVITY;
    }
    a->hit = 0;
#if TRACE
    a->trace[a->trace_idx].x = a->x;
    a->trace[a->trace_idx].y = a->y;
    a->trace[a->trace_idx].z = a->z;
    a->trace[a->trace_idx].init = 1;
    a->trace_idx = (a->trace_idx + 1) % TRACE_LENGTH;
#endif
#if MUTEX
    pthread_mutex_unlock(&(a->mutex));
#endif
    return 0;
}

void particle_draw_trace(struct Particle *a)
{
#if TRACE
#if MUTEX
    pthread_mutex_lock(&(a->mutex));
#endif
    int idx;
    glColor4f(a->color.r, a->color.g, a->color.b, a->color.a);
    glBegin(GL_LINES);
    for (idx = 0; idx < TRACE_LENGTH - 1; idx++)
    {
        int trace_idx = (a->trace_idx + idx) % TRACE_LENGTH;
        int trace_idx_next = (a->trace_idx + idx + 1) % TRACE_LENGTH;
        if ((a->trace[trace_idx].init != 0) && (a->trace[trace_idx_next].init != 0))
        {
            float x = a->trace[trace_idx].x;
            float y = a->trace[trace_idx].y;
            float z = a->trace[trace_idx].z;
            glVertex3f(x, y, z);
            x = a->trace[trace_idx_next].x;
            y = a->trace[trace_idx_next].y;
            z = a->trace[trace_idx_next].z;
            glVertex3f(x, y, z);
        }
    }
    glEnd();
#if MUTEX
    pthread_mutex_unlock(&(a->mutex));
#endif
#endif
}

static void init(void)
{
    int idx;
    int size_of = sizeof(struct Particle);
    particles = malloc(NUM_PARTICLES * size_of);
    for (idx = 0; idx < NUM_PARTICLES; idx++)
    {
        const float r = 3.5f; //get_random_float(2.0f, 5.0f);
        particles[idx].pid = idx;
        particles[idx].x = get_random_float(-WIN_WIDTH_F + r, WIN_WIDTH_F - r);
        particles[idx].y = get_random_float(-WIN_HEIGHT_F + r, WIN_HEIGHT_F - r);
        particles[idx].z = get_random_float(-WIN_DEPTH_F + r, WIN_DEPTH_F - r);
        particles[idx].r = r;
        particles[idx].m = CONST_MASS;
        particles[idx].vx = get_random_float(-1.0f * CONST_SPEED, CONST_SPEED);
        particles[idx].vy = get_random_float(-1.0f * CONST_SPEED, CONST_SPEED);
        particles[idx].vz = get_random_float(-1.0f * CONST_SPEED, CONST_SPEED);
        particles[idx].color.r = get_random_float(0.5f, 1.0f);
        particles[idx].color.g = get_random_float(0.5f, 1.0f);
        particles[idx].color.b = get_random_float(0.5f, 1.0f);
        particles[idx].color.a = 1.0f;
        particles[idx].alive = 1;
        particles[idx].hit = 0;
#if TRACE
        particles[idx].trace_idx = 0;
        particles[idx].trace = malloc(TRACE_LENGTH * sizeof(struct Particle_Trace));
        for (int ndx = 0; ndx < TRACE_LENGTH; ndx++)
        {
            particles[idx].trace[ndx].x = -1.0f;
            particles[idx].trace[ndx].y = -1.0f;
            particles[idx].trace[ndx].z = -1.0f;
            particles[idx].trace[ndx].init = 0;
        }
#endif
#if MUTEX
        pthread_mutex_init(&(particles[idx].mutex), NULL);
#endif
        printf("[%d] x %4.2f y %4.2f r %4.2f Cr %4.2f Cg %4.2f Cb %4.2f Ca %4.2f \n", idx, particles[idx].x, particles[idx].y,
                particles[idx].r, particles[idx].color.r, particles[idx].color.g, particles[idx].color.b, particles[idx].color.a);
    }
}
