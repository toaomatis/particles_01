/*
 * main.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <time.h>

#include <GL/freeglut.h>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    srand(time(0));
    mainwindow(argc, argv);

    return 0;
}
