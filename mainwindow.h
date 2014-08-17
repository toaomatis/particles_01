/*
 * mainwindow.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#define DEBUG_TIMING (0)

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
#if DEBUG_TIMING
enum DEBUG
{
    DEBUG_ON, DEBUG_OFF
};
#endif

extern enum STATES state;
#if DEBUG_TIMING
extern enum DEBUG debug;
#endif

void mainwindow(int argc, char **argv);

#endif /* MAINWINDOW_H_ */
