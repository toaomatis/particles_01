/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "mainwindow.h"
#include "particle.h"

static int init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);
void keyboard(unsigned char key, int x, int y);

static struct Particle *particles;

void mainwindow(int argc, char **argv)
{
    int init_result = init(argc, argv);
    if (init_result != 0)
    {
        fprintf(stderr, "Init was unsuccessful. Quitting '\n");
    }
    glutMainLoop();
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

    return 0;
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
    glutKeyboardFunc(keyboard);
}

void keyboard(unsigned char key, int x, int y)
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
