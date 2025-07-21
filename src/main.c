#define LOG_LEVEL 3
#define LOG_MODULE_NAME "MAIN"
#include "../include/log.h"

#include "../include/queue.h"

int main(){

	queue_t* queue = queue_init(10);
	for(size_t i = 0; i < 16; i ++){
		LOG_DBG("Adding %ld to queue", i);
		queue_add(queue, (void*) i);
	}

	LOG_DBG("Deleting queue");
	queue_dispose(queue);
	return 1;	
}
