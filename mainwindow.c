/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "helper.h"
#include "mainwindow.h"
#include "particle.h"

static int init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);
static void worker(void *ptr);
static void keyboard(unsigned char key, int x, int y);

static struct Particle *particles;
static pthread_t worker_thread;

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
    glutCreateWindow("Tutorial 01");

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
    while (1)
    {
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
        usleep(10000);
    }
    pthread_exit(0); /* exit */
} /* print_message_function ( void *ptr ) */

static void animate()
{

    glutPostRedisplay();
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(animate);
}

static void keyboard(unsigned char key, int x, int y)
{
    exit(0);
}

static void render_scene_cb()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particle_draw(&(particles[i]));
    }
    glutSwapBuffers();
}
