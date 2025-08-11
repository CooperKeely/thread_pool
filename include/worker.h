#ifndef WORKER
#define WORKER

#include <stdlib.h>
#include <assert.h>

#include "arena.h"
#include "channel.h"


typedef enum{
	WORKER_RUNNING,
	WORKER_IDLE,
	WORKER_STOPPED,
	WORKER_ERROR,
} worker_state_t;

typedef struct{
	void* job;
	void* args;
} job_t;

typedef struct {
	size_t id;
	job_t* job;
	channel_t* job_channel;
	worker_state_t state;
} worker_t;


worker_t* worker_init(arena_t* arena, channel_t* chan, size_t id);

int worker_send_job(worker_t* worker, void* job);
int worker_stop(worker_t* worker);
int worker_kill(worker_t* worker);
int worker_join(worker_t* worker);


#endif /* WORKER */
