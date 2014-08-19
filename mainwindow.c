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

const int WIN_WIDTH_I = 768;
const int WIN_HEIGHT_I = 768;
const float WIN_WIDTH_F = 768.0f;
const float WIN_HEIGHT_F = 768.0f;
const float WIN_DEPTH_F = 768.0f;

float scale_width = 768.0f;
float scale_height = 768.0f;
float scale = 1.0f;
/* X = 0, Y = 1, Z = 2 */
int axis = 0;
float theta[3] =
{ 0.0f };

static int init(int argc, char **argv);
static void init_callbacks(void);
static void render_scene_cb(void);
static void displayReshape(int w, int h);
static void special(int key, int x, int y);
static void keyboard(unsigned char key, int x, int y);
static void drawAxis();

static struct Particle *particles;
static int win_num = -1;

enum STATES state = PAUSED;
#if TRACE
enum TRACES traces = TRACES_ON;
#endif
enum INFO info = INFO_ON;
#if DEBUG_TIMING
enum DEBUG debug = DEBUG_ON;
#endif

void mainwindow(int argc, char **argv)
{
    int init_result = init(argc, argv);
    if (init_result != 0)
    {
        fprintf(stderr, "Init was unsuccessful. Quitting '\n");
    }
    glutMainLoop();
    stop_thread_pool();
}

static int init(int argc, char **argv)
{
    GLenum res;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
    glViewport(0, 0, WIN_WIDTH_I, WIN_HEIGHT_I);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    particles = particle();

    start_thread_pool();

    return 0;
}

static void displayReshape(int w, int h)
{
    // If the display is re-sized in any way, the cube is redrawn
    // so that it fits the display properly. Try it!

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1.0f * scale_width, scale_width, -1.0f * scale_height, scale_height, -10.0f, 10.0f);
    //glOrtho(-1.0f * scale_width, scale_width, -1.0f * scale_height, scale_height, -10.0f, 10.0f);
    glOrtho(-1.0f * WIN_WIDTH_F, WIN_WIDTH_F, -1.0f * WIN_HEIGHT_F, WIN_HEIGHT_F, -1.0f * WIN_DEPTH_F, WIN_DEPTH_F);
    glMatrixMode(GL_MODELVIEW);
}

static void animate()
{
#if MUTEX_COND
    pthread_mutex_lock(&(paint_mutex));
    pthread_cond_signal(&(render_cond));
    pthread_cond_wait(&(paint_cond), &(paint_mutex));
#endif
    while (theta[axis] > 360.0f)
    {
        theta[axis] -= 360.0f;
    };
    while (theta[axis] < -360.0f)
    {
        theta[axis] += 360.0f;
    };
    glutPostRedisplay();
#if MUTEX_COND
    pthread_mutex_unlock(&(paint_mutex));
#endif
}

static void init_callbacks()
{
    glutDisplayFunc(render_scene_cb);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(animate);
    glutReshapeFunc(displayReshape);
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
#if TRACE
        case 't':
        {
            traces = (traces == TRACES_OFF ? TRACES_ON : TRACES_OFF);
        }
            break;
#endif
        case 'i':
        {
            info = (info == INFO_OFF ? INFO_ON : INFO_OFF);
        }
            break;
#if DEBUG_TIMING
            case 'd':
            {
                debug = (debug == DEBUG_OFF ? DEBUG_ON : DEBUG_OFF);
            }
            break;
#endif
        case '-':
        {
            scale /= 2.0f;
        }
            break;
        case '+':
        {
            scale *= 2.0f;
        }
            break;
        default:
        {

        }
            break;
    }
    scale_width = WIN_WIDTH_F / scale;
    scale_height = WIN_HEIGHT_F / scale;
}

static void drawAxis()
{
    // save previous matrix
    glPushMatrix();
    // clear matrix
    glLoadIdentity();
    // apply rotations
    glRotatef(theta[0], 1.0f, 0.0f, 0.0f);
    glRotatef(theta[1], 0.0f, 1.0f, 0.0f);
    glRotatef(theta[2], 0.0f, 0.0f, 1.0f);
    // move the axes to the screen corner
    glTranslatef(0.0f, 0.0f, 0.0f);
    /* Add Teapot for scale */
    //glutWireTeapot(WIN_WIDTH_F / 2.0f);
    // draw our axes
    glBegin(GL_LINES);
    // draw line for x axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f * WIN_WIDTH_F, 0.0f, 0.0f);
    glVertex3f(WIN_WIDTH_F, 0.0f, 0.0f);
    // draw line for y axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f * WIN_HEIGHT_F, 0.0f);
    glVertex3f(0.0f, WIN_HEIGHT_F, 0.0f);
    // draw line for Z axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1.0f * WIN_DEPTH_F);
    glVertex3f(0.0f, 0.0f, WIN_DEPTH_F);
    glEnd();
    // load the previous matrix
    glPopMatrix();
}

static void render_scene_cb()
{
#if DEBUG_TIMING
    double secs_set = 0.0f;
    struct timespec in, out;
    int64_t duration = 0;
    clock_gettime(CLOCK_MONOTONIC, &in);
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
    glRotatef(theta[0], 1.0f, 0.0f, 0.0f);
    glRotatef(theta[1], 0.0f, 1.0f, 0.0f);
    glRotatef(theta[2], 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
    drawAxis();
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        glPushMatrix();
        particle_draw(&(particles[i]));
        glPopMatrix();
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
    if (debug == DEBUG_ON)
    {
        printf("render_scene_cb in %f secs (%f Hz) \n", secs_set, 1.0f / secs_set);
    }
#endif
}

static void special(int key, int x, int y)
{
    switch (key)
    {
        /* Rotate Y axis */
        case GLUT_KEY_LEFT:
        {
            theta[1] -= 10.0f;
        }
            break;
        case GLUT_KEY_RIGHT:
        {
            theta[1] += 10.0f;
        }
            break;
            /* Rotate X axis */
        case GLUT_KEY_DOWN:
        {
            theta[0] -= 10.0f;
        }
            break;
        case GLUT_KEY_UP:
        {
            theta[0] += 10.0f;
        }
            break;
            /* Rotate Z axis. DAFUQ */
        case GLUT_KEY_PAGE_DOWN:
        {
            theta[2] -= 10.0f;
        }
            break;
        case GLUT_KEY_PAGE_UP:
        {
            theta[2] += 10.0f;
        }
            break;
    }
    fprintf(stdout, "rot %0.2f %0.2f %0.2f \n", theta[0], theta[1], theta[2]);
}
