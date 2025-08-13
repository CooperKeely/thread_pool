#define LOG_LEVEL 3
#include "../include/log.h"

#include "../include/queue.h"

bool QueueAtCapacity(queue_t* queue){
	return queue->size >= queue->capacity;
}

queue_t* QueueInit(arena_t* arena, size_t capacity){
	LOG_INF("Creating new queue");	

	queue_t* queue = (queue_t*) ArenaPush(arena, sizeof(queue_t));
	assert(queue && "Failed to allocate memory for queue");

	queue->data = (void**)PushArrayZero(arena, void*, capacity);
	assert(queue->data && "Failed to allocate memory for data");	

	queue->capacity = capacity;
	queue->size = 0;
	queue->next = 0;
	return queue;
}

int QueueAdd(queue_t* queue, void* value){
	assert((queue && value) && "Null input");
	if(QueueAtCapacity(queue)){
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

void* QueueRemove(queue_t* queue){
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
		queue->next -= queue->capacity;	
	}
	
	return value;

}

void* QueuePeek(queue_t* queue){
	assert(queue && "Null input");
	if(queue->size <= 0){
		return NULL;
	}
	return queue->data[queue->next];
}


