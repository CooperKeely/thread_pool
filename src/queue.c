#define LOG_LEVEL 3
#define LOG_MODULE_NAME "QUEUE"
#include "../include/log.h"

#include "../include/queue.h"

static inline bool queue_at_capacity(queue_t* queue){
	return queue->capacity <= queue->size;
}

queue_t* queue_init(arena_t* arena, uint32_t capacity){
	LOG_INF("Creating new queue");	

	queue_t* queue = (queue_t*) arena_allocate(arena, sizeof(queue_t));
	assert(queue && "Failed to allocate memory for queue");

	queue->data = (void**) arena_allocate(arena, capacity * sizeof(void*));
	assert(queue->data && "Failed to allocate memory for data");	

	queue->capacity = capacity;
	queue->size = 0;
	queue->next = 0;
	return queue;
}

int queue_add(queue_t* queue, void* value){
	assert(queue && "Null input");
	if(queue_at_capacity(queue)){
		LOG_DBG("Queue at capacity couldn't add: %p", value);
		errno = ENOBUFS;
		return -1;
	}
	
	uint32_t pos = queue->next + queue->size;
	if(pos >= queue->capacity){
		pos -= queue->capacity;	
	}
	
	queue->data[pos] = value;
	queue->size ++;
	return 0;
}

void* queue_remove(queue_t* queue){
	assert(queue && "Null input");

	if(queue->size == 0){
		LOG_ERR("Queue Empty");
		errno = EINVAL;
		return NULL;
	}

	void* value = queue->data[queue->next];
	queue->size--;
	queue->next++;
	if(queue->next>= queue->capacity){
		queue->next-= queue->capacity;	
	}
	return value;

}

void* queue_peek(queue_t* queue){
	assert(queue && "Null input");
	if(queue->size <= 0){
		return NULL;
	}
	return queue->data[queue->next];
}


