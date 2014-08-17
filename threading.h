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

#define MUTEX_COND (0)
#define NUM_THREAD_POOLS (1)

#endif /* THREADING_H_ */
