#ifndef THREAD_POOL
#define THREAD_POOL
#include <stdint.h>
#include <assert.h>

#include "worker.h"
#include "channel.h"
#include "arena.h" 

typedef struct {
	worker_t** workers;
	channel_t* job_channel;
	size_t pool_size;
} thread_pool_t;

thread_pool_t* ThreadPoolInit(arena_t* arena, size_t pool_size);
void ThreadPoolDispatch(thread_pool_t* thread_pool, void*(*job)(void*), void* args);

#endif /*THREAD_POOL*/
