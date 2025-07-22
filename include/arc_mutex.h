#ifndef ARC_MUTEX_H
#define ARC_MUTEX_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <errno.h>

typedef void (*arc_mutex_drop_fn_t)(void*);

typedef struct arc_mutex_t{
	atomic_int ref_count;	
	pthread_mutex_t mutex;
	arc_mutex_drop_fn_t drop;
	void* shared_data;
} arc_mutex_t;

arc_mutex_t* arc_mutex_init(void* data, arc_mutex_drop_fn_t drop_fn);
arc_mutex_t* arc_mutex_clone(arc_mutex_t* arc_mutex);

void arc_mutex_drop(arc_mutex_t* arc_mutex);
void* arc_mutex_get(arc_mutex_t* arc_mutex);
void arc_mutex_lock(arc_mutex_t* arc_mutex);
void arc_mutex_unlock(arc_mutex_t* arc_mutex);

#endif // ARC_MUTEX_H
