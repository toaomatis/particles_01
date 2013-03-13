/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "helper.h"
#include "mainwindow.h"
#include "particle.h"

const int WIN_WIDTH_I = 1024;
const int WIN_HEIGHT_I = 768;
const float WIN_WIDTH_F = 1024.0f;
const float WIN_HEIGHT_F = 768.0f;

static int init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);
static void worker(void *ptr);
static void keyboard(unsigned char key, int x, int y);

#if MUTEX_COND
static pthread_cond_t paint_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t render_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t paint_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static struct Particle *particles;
static pthread_t worker_thread;
static int win_num = -1;

enum STATES
{
    RUNNING, PAUSED, STOPPED
};

enum TRACES
{
    TRACES_ON, TRACES_OFF
};

enum INFO
{
    INFO_ON, INFO_OFF
};

enum DEBUG
{
    DEBUG_ON, DEBUG_OFF
};

enum STATES state = PAUSED;
enum TRACES traces = TRACES_ON;
enum INFO info = INFO_ON;
enum DEBUG debug = DEBUG_OFF;

void mainwindow(int argc, char **argv)
{
    int init_result = init(argc, argv);
    if (init_result != 0)
    {
        fprintf(stderr, "Init was unsuccessful. Quitting '\n");
    }
    glutMainLoop();
    pthread_join(worker_thread, NULL );
}

static int init(int argc, char **argv)
{
    GLenum res;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIN_WIDTH_I, WIN_HEIGHT_I);
    glutInitWindowPosition(100, 100);
    win_num = glutCreateWindow("Tutorial 01");

    init_callbacks();

    // Must be done after glut is initialized!
    res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return -1;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WIN_WIDTH_F, WIN_HEIGHT_F, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);

    particles = particle();

    pthread_create(&worker_thread, NULL, (void *) &worker, (void *) NULL );

    return 0;
}

static void worker(void *ptr)
{
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
            for (int idx = 0; idx < NUM_PARTICLES; idx++)
            {
                for (int ndx = idx + 1; ndx < NUM_PARTICLES; ndx++)
                {
                    particle_interact(&(particles[idx]), &(particles[ndx]));
                }
            }
            for (int idx = 0; idx < NUM_PARTICLES; idx++)
            {
                particle_move(&(particles[idx]));
            }
#if DEBUG_TIMING
            clock_gettime(CLOCK_MONOTONIC, &out);
            duration = timespec_diff_ns(&in, &out);
            secs_set = (double) duration / 1000.0f / 1000.0f / 1000.0f;
            if(debug == DEBUG_ON)
            {
                printf("worker in %f secs (%f Hz) \n", secs_set, 1.0f / secs_set);
            }
#endif
        }
#if MUTEX_COND
        pthread_cond_signal(&(paint_cond));
#endif
    }
#if MUTEX_COND
    pthread_mutex_unlock(&(paint_mutex));
#endif
    pthread_exit(0); /* exit */
} /* print_message_function ( void *ptr ) */

static void animate()
{
#if MUTEX_COND
    pthread_mutex_lock(&(paint_mutex));
    pthread_cond_signal(&(render_cond));
    pthread_cond_wait(&(paint_cond), &(paint_mutex));
#endif
    glutPostRedisplay();
#if MUTEX_COND
    pthread_mutex_unlock(&(paint_mutex));
#endif
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(animate);
}

static void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\e': /* ESC */
        {
            state = STOPPED;
            glutIdleFunc(NULL );
            glutDestroyWindow(win_num);
        }
            break;
        case ' ':
        {
            if (state == RUNNING)
            {
                state = PAUSED;
            }
            else
            {
                state = RUNNING;
            }
        }
            break;
        case 't':
        {
            traces = (traces == TRACES_OFF ? TRACES_ON : TRACES_OFF);
        }
            break;
        case 'i':
        {
            info = (info == INFO_OFF ? INFO_ON : INFO_OFF);
        }
            break;
        case 'd':
        {
            debug = (debug == DEBUG_OFF ? DEBUG_ON : DEBUG_OFF);
        }
            break;

        default:
        {

        }
            break;
    }
}

static void render_scene_cb()
{
#if DEBUG_TIMING
    double secs_set = 0.0f;
    struct timespec in, out;
    int64_t duration = 0;
    clock_gettime(CLOCK_MONOTONIC, &in);
#endif
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particle_draw(&(particles[i]));
#if TRACE
        if (traces == TRACES_ON)
        {
            particle_draw_trace(&(particles[i]));
        }
#endif
    }
    glutSwapBuffers();
#if DEBUG_TIMING
    clock_gettime(CLOCK_MONOTONIC, &out);
    duration = timespec_diff_ns(&in, &out);
    secs_set = (double) duration / 1000.0f / 1000.0f / 1000.0f;
    if(debug == DEBUG_ON)
    {
        printf("render_scene_cb in %f secs (%f Hz) \n", secs_set, 1.0f / secs_set);
    }
#endif
}
