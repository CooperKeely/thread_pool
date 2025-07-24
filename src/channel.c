#define LOG_LEVEL 3
#define LOG_MODULE_NAME "CHANNEL"
#include "../include/log.h"

#include <pthread.h>
#include "../include/channel.h"


channel_t* channel_init(arena_t* arena, uint32_t capacity){
	LOG_INF("Creating new channel");

	channel_t* chan =  arena_allocate(arena, sizeof(channel_t));
	assert(chan && "Failed to allocate memory for channel");

	// create queue
	chan->queue = queue_init(arena, capacity); 
	assert(chan->queue && "Memory allocation failed for queue");

	// create mutex
	int ret = pthread_mutex_init(&chan->mutex, NULL);
	ret = pthread_cond_init(&chan->recv_ready, NULL); 
	ret = pthread_cond_init(&chan->send_ready, NULL); 
	assert((ret == 0) && "Failed to initialize flags or mutex");

	return chan;
}

void channel_cleanup(channel_t* channel){
	assert(channel && "Null input");

	int ret = pthread_mutex_destroy(&channel->mutex);
	ret = pthread_cond_destroy(&channel->recv_ready);
	ret = pthread_cond_destroy(&channel->send_ready);
	assert((ret == 0) && "Failed to destroy flags or mutex");
}


int channel_send(channel_t* channel, void* data){
	assert((channel && channel->queue && data) && "Null input");
	
	// aquire lock
	int ret = pthread_mutex_lock(&channel->mutex);
	assert((ret == 0) && "Mutex lock failed");

	while(queue_at_capacity(channel->queue)){
		ret = pthread_cond_wait(&channel->send_ready, &channel->mutex);
		assert((ret == 0) && "Condition wait failed");
	}

	ret = queue_add(channel->queue, data);
	assert((ret == 0) && "Failed to add to queue");

	ret = pthread_cond_signal(&channel->recv_ready);
	assert((ret == 0) && "Condition signal failed");

	ret = pthread_mutex_unlock(&channel->mutex);
	assert((ret == 0) && "Mutex lock failed");

	return 0;	
}

int channel_recv(channel_t* channel, void** data){
	assert((channel && channel->queue) && "Null input");

	int ret = pthread_mutex_lock(&channel->mutex);
	assert((ret == 0) && "Mutex lock failed");

	while(channel->queue->size == 0){
		ret = pthread_cond_wait(&channel->recv_ready, &channel->mutex);	
		assert((ret == 0) && "Condition wait failed");
	}
	*data = queue_remove(channel->queue);
	assert((*data != NULL) && "Null data removed from queue");

	ret = pthread_cond_signal(&channel->send_ready);		
	assert((ret == 0) && "Condition signal failed");

	ret = pthread_mutex_unlock(&channel->mutex);
	assert((ret == 0) && "Mutex lock failed");

	return 0;
}


