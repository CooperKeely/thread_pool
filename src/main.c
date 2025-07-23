#define LOG_LEVEL 3
#define LOG_MODULE_NAME "MAIN"
#include "../include/log.h"

#include <stdlib.h>
#include <pthread.h>

#include "../include/channel.h"

#define THREADS 4

void *worker(void* args){
	channel_t* chan = (channel_t *) args;
	LOG_DBG("worker created");
	void* data = NULL; 

	while(true){
		int ret = channel_recv(chan, &data);
		if(ret < 0){
			LOG_WRN("Failed to recieve data");
			continue ;
		}
		if(data == NULL){
			LOG_WRN("Recieved null data");
			continue;
		}
		int* value = (int*) data;
		if(*value < 0){
			LOG_INF("Termination message recieved");
			return NULL;
		}
		LOG_INF("Recieved: %d", *value);
		free(value);
	}
	return NULL;
}


int main() {
	channel_t* chan = channel_init(100);

	LOG_DBG("Channel has been created, dispatching threads");

	for(int i = 0; i < 80; i ++){
		int* test = malloc(sizeof(int));
		*test = i;
		int ret = channel_send(chan, (void *) test);
		if(ret < 0){
			LOG_WRN("Failed to send data over channel");
		}
	}	

	// Launch threads
	pthread_t threads[THREADS];
	for (int i = 0; i < THREADS; ++i) {
		pthread_create(&threads[i], NULL, worker, chan);
	}
	
	LOG_DBG("Data sent waiting on threads");

	for(int i = -4; i < 0; i ++){
		int* test = malloc(sizeof(int));
		*test = i;
		int ret = channel_send(chan, (void *) test);
		if(ret < 0){
			LOG_WRN("Failed to send data over channel");
		}
	}

	// Wait for all threads to complete
	for (int i = 0; i < THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	LOG_DBG("Threads have been joined destorying channel");
	channel_dispose(chan);

	
	return 0;
}

