/*
 * helper.h
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <inttypes.h>
#include <time.h>

#define NSEC_PER_SEC (1000000000)
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

void timespec_add(struct timespec *t1, const struct timespec *t2);
void timespec_add_ns(struct timespec *t1, const long nsec);
//! @brief Shamelessly stolen from the kernel.
int64_t timespec_diff_ns(struct timespec *start, struct timespec *end);
struct timespec timespec_diff(struct timespec *start, struct timespec *end);

float get_random_float(const float min, const float max);

#endif /* HELPER_H_ */
