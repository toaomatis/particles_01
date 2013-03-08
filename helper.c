/*
 * helper.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdlib.h>

float get_random_float()
{
    const float min_val = 10.0f;
    const float max_val = 25.0f;
    const float delta = max_val - min_val;
    float value = min_val + (((float) rand() / RAND_MAX) * delta);
    return value;
}
