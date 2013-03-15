/*
 * threading.c
 *
 *  Created on: Mar 15, 2013
 *      Author: mathijs
 */

#include <curses.h>
#include <math.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "helper.h"
#include "mainwindow.h"
#include "particle.h"
#include "threading.h"

#if MUTEX_COND
static pthread_cond_t paint_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t render_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t paint_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static struct Particle *particles;

struct worker_args
{
    int tid;
    uint64_t base_start;
    uint64_t base_stop;
    int base_size;
};
struct worker_args *wrkr_args ;
static pthread_t tids[THREAD_POOL_SIZE];


static void *worker(void *ptr);

void start_thread_pool()
{
    int idx;
    particles = particle();
    /* NUM_PARTICLES is zero indexed */
    wrkr_args = malloc(THREAD_POOL_SIZE * sizeof(struct worker_args));
    /* NUM_PARTICLES / sqrt(THREAD_POOL_SIZE / idx) */
    for (idx = 0; idx < THREAD_POOL_SIZE; idx++)
    {
        wrkr_args[idx].tid = idx;
        wrkr_args[idx].base_start = NUM_PARTICLES - MIN(ceil((double)NUM_PARTICLES * sqrt((double)(THREAD_POOL_SIZE - idx) / (double)THREAD_POOL_SIZE)), NUM_PARTICLES);
        wrkr_args[idx].base_stop = NUM_PARTICLES - MIN(ceil((double)NUM_PARTICLES * sqrt((double)(THREAD_POOL_SIZE - idx - 1) / (double)THREAD_POOL_SIZE)), NUM_PARTICLES);
        wrkr_args[idx].base_size = NUM_PARTICLES;
        if (wrkr_args[idx].base_start < wrkr_args[idx].base_stop)
        {
            pthread_create(&(tids[idx]), NULL, &worker, (void *) &(wrkr_args[idx]));
            //sleep(5);
        }
        else
        {
            wrkr_args[idx].tid = -1;
        }
    }
}

void stop_thread_pool()
{
    int idx;
    for (idx = 0; idx < THREAD_POOL_SIZE; idx++)
    {
        if (wrkr_args[idx].tid != -1)
        {
            pthread_join(tids[idx], NULL ); /* Wait until thread is finished */
        }
    }
    free(wrkr_args);
}

static void *worker(void *ptr)
{
    struct worker_args *args = (struct worker_args *) ptr;
#if MUTEX_COND
    pthread_mutex_lock(&(paint_mutex));
#endif
    while (state != STOPPED)
    {
#if MUTEX_COND
        pthread_cond_wait(&(render_cond), &(paint_mutex));
#endif
        if (state == RUNNING)
        {
#if DEBUG_TIMING
            double secs_set = 0.0f;
            struct timespec in, out;
            int64_t duration = 0;
            clock_gettime(CLOCK_MONOTONIC, &in);
#endif
            int test = 0;
            for (int idx = args->base_start; idx < args->base_stop; idx++)
            {
                for (int ndx = idx + 1; ndx < NUM_PARTICLES; ndx++)
                {
                    particle_interact(&(particles[idx]), &(particles[ndx]));
                    test++;
                }
            }
            for (int idx = args->base_start; idx < args->base_stop; idx++)
            {
                particle_move(&(particles[idx]));
            }
#if DEBUG_TIMING
            clock_gettime(CLOCK_MONOTONIC, &out);
            duration = timespec_diff_ns(&in, &out);
            secs_set = (double) duration / 1000.0f / 1000.0f / 1000.0f;
            if (debug == DEBUG_ON)
            {
                printf("[%d]worker in %f secs (%f Hz) iters %d \n", args->tid, secs_set, 1.0f / secs_set, test);
            }
#endif
        }
        else
        {
            usleep(1000);
        }
#if MUTEX_COND
        pthread_cond_signal(&(paint_cond));
#endif
    }
#if MUTEX_COND
    pthread_mutex_unlock(&(paint_mutex));
#endif
    pthread_exit(0); /* exit */
    return NULL;
} /* print_message_function ( void *ptr ) */
