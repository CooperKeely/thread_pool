#include "../include/thread_pool.h" 

thread_pool_t* ThreadPoolInit(arena_t* arena, size_t pool_size){
	size_t i;
	thread_pool_t* thread_pool = ArenaPush(arena, sizeof(thread_pool_t));
	
	thread_pool->pool_size = pool_size;
	thread_pool->job_channel = ChannelInit(arena, 2 * pool_size);
	thread_pool->workers = ArenaPush(arena, pool_size * sizeof(worker_t*));	

	for(i = 0; i < pool_size; i ++){
		thread_pool->workers[i] = WorkerInit(arena, thread_pool->job_channel, i);
	}
	
	for(i = 0; i < pool_size; i ++){
		WorkerStart(thread_pool->workers[i]);
	}

	return thread_pool;
}

void ThreadPoolDispatch(thread_pool_t* thread_pool, void*(*job)(void*), void* args){
	job_t* job_struct = malloc(sizeof(job_t));
	job_struct->job = job;
	job_struct->args = args;
	void* data = (void*) job_struct;
	ChannelSend(thread_pool->job_channel, data);
}


