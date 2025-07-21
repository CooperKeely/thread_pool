#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

typedef struct queue_t{
	uint32_t size;
	uint32_t next;
	uint32_t capacity;
	void** data;	
} queue_t;

queue_t* queue_init(uint32_t capacity);
void queue_dispose(queue_t* queue);
int queue_add(queue_t* queue, void* data);
void* queue_remove(queue_t* queue);
void* queue_peek(queue_t* queue);

#endif // QUEUE_H
