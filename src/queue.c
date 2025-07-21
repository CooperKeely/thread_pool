#define LOG_LEVEL 3
#define LOG_MODULE_NAME "QUEUE"
#include "../include/log.h"

#include "../include/queue.h"

static inline bool queue_at_capacity(queue_t* queue){
	return queue->size >= queue->capacity;
}

queue_t* queue_init(uint32_t capacity){
	LOG_INF("Creating new queue");	
	queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
	void** data = (void**) malloc(capacity * sizeof(void*));
	if(!queue || !data){
		LOG_ERR("Failed to allocate memory");
		free(queue);
		free(data);
		errno = ENOMEM;
		return NULL;
	}

	queue->capacity = capacity;
	queue->size = 0;
	queue->next = 0;
	queue->data = data;
	return queue;
}

void queue_dispose(queue_t* queue){
	LOG_INF("Deleting queue");
	free(queue->data);
	free(queue);
}

int queue_add(queue_t* queue, void* value){
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
	void* value = NULL;

	if(queue->size <= 0){
		LOG_ERR("Queue size is less than or equal to zero");
		errno = EINVAL;
		return value;
	}

	value = queue->data[queue->next];
	queue->size--;
	queue->next++;
	if(queue->next>= queue->capacity){
		queue->next-= queue->capacity;	
	}
	return value;

}

void* queue_peek(queue_t* queue){
	if(queue->size <= 0){
		LOG_ERR("Queue size is less than or equal to zero");
		errno = EINVAL;
		return NULL;
	}
	return queue->data[queue->next];
}

