/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

static void init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);

void mainwindow(int argc, char **argv)
{
    init(argc, argv);
}

static void init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 01");

    init_callbacks();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
}

static void render_scene_cb()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}
