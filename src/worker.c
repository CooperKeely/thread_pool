#include "../include/worker.h"

worker_t* WorkerInit(arena_t* arena, channel_t* chan, pthread_t id){
	assert((arena && chan) && "Null inputs");

	worker_t* worker = ArenaPush(arena, sizeof(worker_t));
	assert(worker && "Allocation failed");

	worker->id = id;
	worker->state = WORKER_IDLE;
	worker->job_channel = chan;	
	return worker;
}

int WorkerStart(worker_t* worker){
	assert(worker && "Null input worker pointer");
	while(1){
		void* data = NULL;
		void* args = NULL;
		void* (*job)(void*) = NULL;
		job_t* job_struct = NULL;

		worker->state = WORKER_IDLE;

		ChannelReceive(worker->job_channel, &data);
		assert(data && "Job struct recieved is null");	

		job_struct = (job_t*) data;
		job = job_struct->job;
		args = job_struct->args;
		
		assert(job && "Function pointer is null");
		worker->state = WORKER_RUNNING;
		job(args);
	}
	return 1;
}


/* stops the worker gracecully*/
int WorkerStop(worker_t* worker){
	assert(worker && "Null input worker pointer");
	return -1;
}

/* kills the worker ungracefully */
int WorkerKill(worker_t* worker){
	assert(worker && "Null input worker pointer");
	return -1;
}

/* retrieves worker's last return value */
int WorkerJoin(worker_t* worker){
	assert(worker && "Null input worker pointer");
	return -1;
}


