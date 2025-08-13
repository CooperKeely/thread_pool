#ifndef WORKER
#define WORKER

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "arena.h"
#include "channel.h"


typedef enum{
	WORKER_RUNNING,
	WORKER_IDLE,
	WORKER_STOPPED,
	WORKER_ERROR,
} worker_state_t;

typedef struct{
	void*(*job)(void*);
	void* args;
} job_t;

typedef struct {
	pthread_t id;
	channel_t* job_channel;
	worker_state_t state;
} worker_t;


worker_t* WorkerInit(arena_t* arena, channel_t* chan, size_t id);

/* starts worker execution */
int WorkerStart(worker_t* worker);

/* stops the worker gracecully*/
int WorkerStop(worker_t* worker);

/* kills the worker ungracefully */
int WorkerKill(worker_t* worker);

/* retrieves worker's last return value */
int WorkerJoin(worker_t* worker);


#endif /* WORKER */
