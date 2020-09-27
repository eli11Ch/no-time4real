/*
 * Author: Jérôme Pouiller <jerome@sysmic.fr>
 * Created: Fri Dec 11 22:57:13 CET 2009
 * Licence: GPL
 *
 * Define interface for timer services.
 */

#ifndef MYTIMER_H
#define MYTIMER_H
#include <pthread.h>
#include <time.h>

/* Helper to define a task */
typedef void (*task_t)();

/* INSERT YOUR mytimer_s DEFINITION HERE */

/* Contains all needed data */
typedef struct mytimer_s mytimer_t;

/* 
 * All following functions return
 *  - 0 on success
 *  - another value on error
 */

/* 
 * Initialize new timer instance. 
 * Instance is returned using parameter out. 
 * User have to call this function first 
 */
int mytimer_run(mytimer_t *out);

/* Stop timer instance */
int mytimer_stop(mytimer_t *mt);

/* Add a task to timer. Task will be executed at time tp */
int mytimer_add_absolute(mytimer_t *mt, task_t t, const struct timespec *tp);

/* Add a task to timer. Task will be executed in ms milliseconds */
int mytimer_add_msecond(mytimer_t *mt, task_t t, unsigned long ms);

/* Add a task to timer. Task will be executed in s seconds */
int mytimer_add_second(mytimer_t *mt, task_t t, unsigned long sec);

/* Add a task to timer. Task will be executed in h hours */
int mytimer_add_hour(mytimer_t *mt, task_t t, unsigned long h);

/* Remove first occurence of task t from timer. */
int mytimer_remove(mytimer_t *mt, task_t t);


#endif /* MYTIMER_H */
