/*
 * helper.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdlib.h>

float get_random_float(const float min, const float max)
{
    const float delta = max - min;
    float value = min + (((float) rand() / RAND_MAX) * delta);
    return value;
}
