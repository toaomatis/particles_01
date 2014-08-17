/*
 * mainwindow.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <curses.h>
#include <math.h>
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
#include "threading.h"

const int WIN_WIDTH_I = 1024;
const int WIN_HEIGHT_I = 768;
const float WIN_WIDTH_F = 1024.0f;
const float WIN_HEIGHT_F = 768.0f;

static void animate(void);
static void render_scene_cb(void);
static float scale = 1.0f;
static float dest_w = 1024.0f;
static float dest_h = 768.0f;

static float eye_x = 0;
static float eye_y = 0;

static void keyboard(unsigned char key, int x, int y);

static struct Particle *particles;
static int win_num = -1;

enum STATES state = PAUSED;
enum TRACES traces = TRACES_ON;
enum INFO info = INFO_ON;
enum DEBUG debug = DEBUG_ON;

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

static void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            eye_x -= 100;
            break;
        case GLUT_KEY_RIGHT:
            eye_x += 100;
            break;
        case GLUT_KEY_UP:
            eye_y += 100;
            break;
        case GLUT_KEY_DOWN:
            eye_y -= 100;
            break;
    }
    fprintf(stdout, "Eye %0.2f %0.2f\n", eye_x, eye_y);
}

static void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\e': /* ESC */
        {
            state = STOPPED;
            glutIdleFunc(NULL);
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
        case '+':
        {
            scale *= 1.1;
        }
            break;
        case '-':
        {
            scale /= 1.1f;
        }
            break;
        default:
        {

        }
            break;
    }
    dest_w = WIN_WIDTH_F * scale;
    dest_h = WIN_HEIGHT_F * scale;
    fprintf(stdout, "Scale %0.2f Width %0.2f Height %0.2f\n", scale, dest_w, dest_h);
}

static void render_scene_cb()
{
#if DEBUG_TIMING
    double secs_set = 0.0f;
    struct timespec in, out;
    int64_t duration = 0;
    clock_gettime(CLOCK_MONOTONIC, &in);
#endif
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    //gluLookAt(WIN_WIDTH_F / 2.0f, WIN_HEIGHT_F / 2.0f, -1.0f, WIN_WIDTH_F / 2.0f, WIN_HEIGHT_F / 2.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    //gluLookAt(dest_w / 2.0f, dest_h / 2.0f, -1.0f, dest_w / 2.0f, dest_h / 2.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    //gluLookAt(WIN_WIDTH_F , 1.0f, -1.0f, 0, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    //gluLookAt(WIN_WIDTH_F , 0.0, -1.0f, WIN_WIDTH_F * scale, 0.0, 1.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        struct Particle *p = &(particles[i]);
        glPushMatrix();
        glTranslatef(p->x, p->y, 0.1f);
        particle_draw(p);
        glPopMatrix();
#if TRACE
        if (traces == TRACES_ON)
        {
            particle_draw_trace();
        }
#endif
    }
    glutSwapBuffers();
#if DEBUG_TIMING
    clock_gettime(CLOCK_MONOTONIC, &out);
    duration = timespec_diff_ns(&in, &out);
    secs_set = (double) duration / 1000.0f / 1000.0f / 1000.0f;
    if (debug == DEBUG_ON)
    {
        printf("render_scene_cb in %f secs (%f Hz) \n", secs_set, 1.0f / secs_set);
    }
#endif
}

void changeSize(int w, int h)
{

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio = ((float) w / (float) h);

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    glFrustum(0.0f, WIN_WIDTH_F, 0.0f, WIN_HEIGHT_F, -1.0f, 10.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    // Reset Matrix
    glLoadIdentity();
}

void mainwindow(int argc, char **argv)
{
    GLenum res;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIN_WIDTH_I, WIN_HEIGHT_I);
    win_num = glutCreateWindow("Tutorial 01");
    // Must be done after glut is initialized!
    res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return;
    }

    glutDisplayFunc(render_scene_cb);
    glutReshapeFunc(changeSize);
    glutIdleFunc(animate);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    particles = particle();
    start_thread_pool();

    glutMainLoop();

    stop_thread_pool();
}
