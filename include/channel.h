#ifndef CHANNEL_H
#define CHANNEL_H

#include <pthread.h>
#include <errno.h>

#include "queue.h"
#include "arena.h"

typedef struct channel_t {
	queue_t* queue;
	pthread_mutex_t mutex;	
	pthread_cond_t send_ready;
	pthread_cond_t recv_ready;
} channel_t;

channel_t* ChannelInit(arena_t* arena, uint32_t capacity);
void ChannelCleanup(channel_t* channel);
int ChannelSend(channel_t* channel, void* data);
int ChannelReceive(channel_t* channel, void** data);

#endif /*CHANNEL_H*/
