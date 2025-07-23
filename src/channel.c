#include <pthread.h>
#define LOG_LEVEL 3
#define LOG_MODULE_NAME "CHANNEL"
#include "../include/log.h"

#include "../include/channel.h"

// Private helper functions

static void safe_mutex_lock(pthread_mutex_t* mutex){
	int ret = pthread_mutex_lock(mutex);	
	if(ret == 0) return; // mutex lock aquired return
	
	// an error occured
	switch (ret){
		case EBUSY:
			LOG_ERR("Mutex already locked");
			break;
		case EINVAL:
			LOG_ERR("Invalid mutex ");
			break;
		case EAGAIN:
			LOG_ERR("Maximum number of recursive locks exceded");
			break;
		case EDEADLK:
			LOG_ERR("Mutex in deadlock");
			break;
		case EPERM:
			LOG_ERR("Thread does not own mutex");
			break;
	}
	exit(EXIT_FAILURE);
}

static void safe_mutex_unlock(pthread_mutex_t* mutex){
	int ret = pthread_mutex_unlock(mutex);	
	if(ret == 0) return; // mutex unlocked return
	
	// an error occured
	switch (ret){
		case EINVAL:
			LOG_ERR("Invalid mutex");
			break;
		case EPERM:
			LOG_ERR("Thread does not own mutex");
			break;
	}
	exit(EXIT_FAILURE);
}


// Public API

channel_t* channel_init(arena_t* arena, uint32_t capacity){
	LOG_INF("Creating new channel");
	channel_t* chan =  arena_allocate(arena, sizeof(channel_t));
	assert(chan && "Failed to allocate memory for channel");

	// create queue
	chan->queue = queue_init(arena, capacity); 
	assert(chan->queue && "Memory allocation failed for queue");

	// create mutex
	int ret = pthread_mutex_init(&chan->mutex, NULL);
	assert((ret >= 0) && "Failed to initialize mutex");
	
	// create condition
	ret = pthread_cond_init(&chan->flag, NULL); 
	assert((ret >= 0) && "Failed to initialize condition");

	return chan;
}

void channel_cleanup(channel_t* channel){
	assert(channel && "Null input");

	int ret = pthread_mutex_destroy(&channel->mutex);
	assert((ret >= 0) && "Failed to destory mutex");

	ret = pthread_cond_destroy(&channel->flag);
	assert((ret >= 0) && "Failed to destroy flag");
}

// TODO:: make send and recv async with a pthread flag to notify the reciving end of the channel 

int channel_send(channel_t* channel, void* data){
	assert((channel && channel->queue && data) && "Null input");

	safe_mutex_lock(&channel->mutex);

	int ret = queue_add(channel->queue, data);
	if (ret >= 0) {
		pthread_cond_signal(&channel->flag);
	}

	safe_mutex_unlock(&channel->mutex);

	if (ret < 0){
		if(errno == ENOBUFS) LOG_WRN("Channel is full");
		return -1;
	}

	return 1;	
}

int channel_recv(channel_t* channel, void** data){
	assert((channel && channel->queue) && "Null input");

	pthread_mutex_lock(&channel->mutex);

	while(queue_peek(channel->queue) == NULL){
		pthread_cond_wait(&channel->flag, &channel->mutex);	
	}
	*data = queue_remove(channel->queue);
	
	pthread_mutex_unlock(&channel->mutex);

	if(*data == NULL){
		if (errno == EINVAL) LOG_ERR("Queue size error");	
		else LOG_ERR("An unknown error occured");
		return -1;
	}
	return 1;
}


