/*
 * mainwindow.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#define DEBUG_TIMING (1)

extern const int WIN_WIDTH_I;
extern const int WIN_HEIGHT_I;
extern const float WIN_WIDTH_F;
extern const float WIN_HEIGHT_F;

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

extern enum STATES state;
extern enum TRACES traces;
extern enum INFO info;
extern enum DEBUG debug;

void mainwindow(int argc, char **argv);

#endif /* MAINWINDOW_H_ */
