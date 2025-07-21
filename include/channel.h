#ifndef CHANNEL_H
#define CHANNEL_H

#include <pthread.h>
#include <errno.h>
#include "queue.h"

typedef struct chan_t {
	queue_t* queue;
	
} chan_t;

chan_t* channel_init(uint32_t capacity);
void channel_dispose(chan_t* channel);
int channel_send(chan_t* channel, void* data);
int channel_recv(chan_t* channel, void** data);

#endif //CHANNEL_H
