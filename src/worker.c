#include "../include/worker.h"

worker_t* worker_init(arena_t* arena, channel_t* chan, size_t id){
	assert((arena && chan) && "Null inputs");

	worker_t* worker = arena_allocate(arena, sizeof(worker_t));
	assert(worker && "Allocation failed");

	worker->id = id;
	worker->state = WORKER_IDLE;
	worker->job = NULL;
	worker->job_channel = chan;	
	return worker;
}

int worker_send_job(worker_t* worker, void* job);
int worker_stop(worker_t* worker);
int worker_kill(worker_t* worker);
int worker_join(worker_t* worker);
