/*
 * helper.c
 *
 *  Created on: Mar 8, 2013
 *      Author: mathijs
 */

#include <stdlib.h>
#include <assert.h>
#include "helper.h"

/* Add timespec t2 to t1
 *
 * Both t1 and t2 must already be normalized
 * i.e. 0 <= nsec < 1000000000 */
void timespec_add(struct timespec *t1, const struct timespec *t2)
{
  assert(t1->tv_nsec >= 0);
  assert(t1->tv_nsec < NSEC_PER_SEC);
  assert(t2->tv_nsec >= 0);
  assert(t2->tv_nsec < NSEC_PER_SEC);
  t1->tv_sec += t2->tv_sec;
  t1->tv_nsec += t2->tv_nsec;
  if (t1->tv_nsec >= NSEC_PER_SEC)
  {
    t1->tv_sec++;
    t1->tv_nsec -= NSEC_PER_SEC;
  }
  else if (t1->tv_nsec < 0)
  {
    t1->tv_sec--;
    t1->tv_nsec += NSEC_PER_SEC;
  }
}
void timespec_add_ns(struct timespec *t, const long nsec)
{
    t->tv_sec += nsec / NSEC_PER_SEC;
    t->tv_nsec += nsec % NSEC_PER_SEC;
    while (t->tv_nsec > NSEC_PER_SEC)
    {
        t->tv_sec++;
        t->tv_nsec -= NSEC_PER_SEC;
    }

}

/*
 * timespec_diff_ns - Return difference of two timestamps in nanoseconds
 * In the rare case of @end being earlier than @start, return zero
 */
int64_t timespec_diff_ns(struct timespec *start, struct timespec *end)
{
    int64_t ret;

    ret = (int64_t)(end->tv_sec - start->tv_sec)*NSEC_PER_SEC;
    ret += (int64_t)(end->tv_nsec - start->tv_nsec);
    if (ret < 0)
        return 0;
    return ret;
}
/*
 * timespec_diff_ns - Return difference of two timestamps in nanoseconds
 * In the rare case of @end being earlier than @start, return zero
 */
struct timespec timespec_diff(struct timespec *start, struct timespec *end)
{
  struct timespec ret;

  ret.tv_sec = (end->tv_sec - start->tv_sec);
  ret.tv_nsec= (end->tv_nsec - start->tv_nsec);
  if(ret.tv_nsec<0)
  {
    ret.tv_nsec += NSEC_PER_SEC;
    ret.tv_sec--;
  }
  return ret;
}

float get_random_float_ranged(const float min, const float max)
{
    const float delta = max - min;
    float value = min + (((float) rand() / RAND_MAX) * delta);
    return value;
}
