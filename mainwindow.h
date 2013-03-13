/*
 * mainwindow.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#define MUTEX_COND (0)
#define DEBUG_TIMING (1)
#define THREAD_POOL_SIZE (8)

extern const int WIN_WIDTH_I;
extern const int WIN_HEIGHT_I;
extern const float WIN_WIDTH_F;
extern const float WIN_HEIGHT_F;

void mainwindow(int argc, char **argv);

#endif /* MAINWINDOW_H_ */
