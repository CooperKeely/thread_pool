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

static int safe_channel_check(channel_t* channel){
	if(channel == NULL){
		errno = EINVAL;
		LOG_ERR("Channel is a null pointer");
		return -1;
	}
	if(channel->queue == NULL){
		errno = EINVAL;
		LOG_ERR("Channel queue is a null pointer");
		return -1;
	}
	if(channel->mutex == NULL){
		errno = EINVAL;
		LOG_ERR("Channel mutex is a null pointer");
		return -1;
	}
	return 0;
}

// Public API

channel_t* channel_init(uint32_t capacity){
	LOG_INF("Creating new channel");
	
	// create queue
	queue_t* queue = queue_init(capacity); 
	if(queue == NULL){
		if(errno == ENOMEM) LOG_ERR("Couldn't allocate memory for queue");
		else LOG_ERR("An unknown fault occured");
		return NULL;
	}
	
	// create mutex
	pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
	int ret = pthread_mutex_init(mutex, NULL);
	if(ret < 0){
		if(ret == ENOMEM) LOG_ERR("Couldn't allocate memory for mutex");
		else if(ret == EBUSY) LOG_ERR("Initialiation of an initialized mutex occured");
		else LOG_ERR("An unknown error occured");
		return NULL;
	}

	channel_t* chan = (channel_t*) malloc(sizeof(channel_t));
	if(chan == NULL){
		LOG_ERR("Couldn't allocate memory for channel");
		errno = ENOMEM;
		return NULL;
	}

	chan->queue = queue;
	chan->mutex = mutex;
	return chan;
}

void channel_dispose(channel_t* channel){
	if(channel == NULL){
		errno = EINVAL;
		LOG_ERR("Channel is a null pointer");
		return;
	}

	if(channel->queue == NULL){
		errno = EINVAL;
		LOG_ERR("Channel queue is a null pointer");
	}else{
		free(channel->queue);
		channel->queue = NULL;
	}

	if(channel->mutex == NULL){
		errno = EINVAL;
		LOG_ERR("Channel mutex is a null pointer");
	}else{
		int ret = pthread_mutex_destroy(channel->mutex);
		if (ret < 0){
			if(ret == EBUSY) LOG_ERR("Attempt to destory mutex with an active reference");
			else if (ret == EINVAL) LOG_ERR("Invalid mutex");
			else LOG_ERR("An unknown error occured");
		}
		free(channel->mutex);
		channel->mutex = NULL;
	}

	free(channel);
	channel = NULL;
}

// TODO:: make send and recv async with a pthread flag to notify the reciving end of the channel 

int channel_send(channel_t* channel, void* data){
	int ret = safe_channel_check(channel);
	if(ret != 0) return -1;

	safe_mutex_lock(channel->mutex);
	ret = queue_add(channel->queue, data);
	safe_mutex_unlock(channel->mutex);

	if (ret < 0){
		if(errno == ENOBUFS) LOG_WRN("Channel is full");
		return -1;
	}
	return 1;	
}

int channel_recv(channel_t* channel, void** data){
	int ret = safe_channel_check(channel);
	if(ret != 0) return -1;

	safe_mutex_lock(channel->mutex);
	while (queue_peek(channel->queue) != NULL){
		data = queue_remove(channel->queue);
	}
	safe_mutex_unlock(channel->mutex);

	if(data == NULL){
		if (errno == EINVAL) LOG_ERR("Queue size error");	
		else LOG_ERR("An unknown error occured");
		return -1;
	}
	return 1;
}


