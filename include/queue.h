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

queue_t* queue_init(arena_t* arena, size_t capacity);
int queue_add(queue_t* queue, void* data);
void* queue_remove(queue_t* queue);
void* queue_peek(queue_t* queue);
bool queue_at_capacity(queue_t* queue);
bool queue_at_empty(queue_t* queue);
#endif /* QUEUE_H */
