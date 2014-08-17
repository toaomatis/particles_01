/*
 * threading.h
 *
 *  Created on: Mar 15, 2013
 *      Author: mathijs
 */

#ifndef THREADING_H_
#define THREADING_H_

void start_thread_pool(void);
void stop_thread_pool(void);

#define MUTEX_COND (1)
#define NUM_THREAD_POOLS (1)

#if MUTEX_COND
extern pthread_cond_t paint_cond;
extern pthread_cond_t render_cond;
extern pthread_mutex_t paint_mutex;
#endif

#endif /* THREADING_H_ */
