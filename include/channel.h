#ifndef CHANNEL_H
#define CHANNEL_H

#include <pthread.h>
#include <errno.h>
#include "queue.h"

typedef struct channel_t {
	queue_t* queue;
	pthread_mutex_t* mutex;	
	pthread_cond_t* flag;
} channel_t;

channel_t* channel_init(uint32_t capacity);
void channel_dispose(channel_t* channel);
int channel_send(channel_t* channel, void* data);
int channel_recv(channel_t* channel, void** data);

#endif //CHANNEL_H
