#ifndef ARC_MUTEX_H
#define ARC_MUTEX_H

#include <pthread.h>
#include <stdatomic.h>

typedef struct arc_mutex_t{
	atomic_int_fast32_t ref_count;	
	pthread_mutex_t mutex;
	void* shared_data;
	void destroy_data(void*);
} arc_mutex_t;

arc_mutex_t* arc_mutex_init(void* destroy_function, void* data);



#endif // ARC_MUTEX_H
