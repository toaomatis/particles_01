/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "particle.h"

static int init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);
static void create_vertex_buffer(void);

static GLuint VBO;
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
    glutInitWindowSize(1024, 768);
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

    particles = particle();

    return 0;
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
}

static void render_scene_cb()
{
    glClear(GL_COLOR_BUFFER_BIT);
#if 0
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, 2);

    glDisableVertexAttribArray(0);
#endif
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particle_draw(&(particles[i]));
    }

    glutSwapBuffers();
}

static void create_vertex_buffer()
{
#if 0
    glGenBuffers(size, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct Particle), particles, GL_STATIC_DRAW);
#endif
}
