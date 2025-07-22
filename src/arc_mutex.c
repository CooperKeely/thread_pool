#include <pthread.h>
#define LOG_LEVEL 3
#define LOG_MODULE_NAME "ARC_MUTEX"
#include "../include/log.h"

#include "../include/arc_mutex.h"

arc_mutex_t* arc_mutex_init(void* data, arc_mutex_drop_fn_t drop_fn){
	arc_mutex_t* arc_mutex = (arc_mutex_t*) malloc(sizeof(arc_mutex_t));
	if(arc_mutex == NULL){
		LOG_ERR("Couldn't allocate Memory");
		errno = ENOMEM;
		return NULL;
	}
	arc_mutex->drop = drop_fn;
	arc_mutex->shared_data = data;
	atomic_init(&arc_mutex->ref_count, 1);
	LOG_DBG("[init] ref count: %d", 1);
	return arc_mutex;
}

arc_mutex_t* arc_mutex_clone(arc_mutex_t* arc_mutex){
	int new = atomic_fetch_add(&arc_mutex->ref_count, 1) + 1;
	LOG_DBG("[clone] ref count: %d", new);
	return arc_mutex;
}

void arc_mutex_drop(arc_mutex_t* arc_mutex){
	int new = atomic_fetch_sub(&arc_mutex->ref_count, 1) - 1;

	LOG_DBG("[drop] ref count: %d", new);
	if (new != 0){
		return;
	}
	if (arc_mutex->drop && arc_mutex->shared_data){
		arc_mutex->drop(arc_mutex->shared_data);
	}
	pthread_mutex_destroy(&arc_mutex->mutex);
	free(arc_mutex);
	arc_mutex = NULL;
}

void* arc_mutex_get(arc_mutex_t* arc_mutex){
	return &arc_mutex->shared_data;
}

void arc_mutex_lock(arc_mutex_t* arc_mutex){
	pthread_mutex_lock(&arc_mutex->mutex);
}

void arc_mutex_unlock(arc_mutex_t* arc_mutex){
	pthread_mutex_unlock(&arc_mutex->mutex);
}

