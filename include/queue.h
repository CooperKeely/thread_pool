#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include "arena.h"


typedef struct queue_t{
	size_t size;
	size_t next;
	size_t capacity;
	void** data;	
} queue_t;

queue_t* QueueInit(arena_t* arena, size_t capacity);
int QueueAdd(queue_t* queue, void* data);
void* QueueRemove(queue_t* queue);
void* QueuePeek(queue_t* queue);
bool QueueAtCapacity(queue_t* queue);
bool QueueEmpty(queue_t* queue);
#endif /* QUEUE_H */
