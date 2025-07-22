#define LOG_LEVEL 3
#define LOG_MODULE_NAME "MAIN"
#include "../include/log.h"

#include <stdlib.h>
#include <pthread.h>

#include "../include/channel.h"

#define THREADS 4

void *worker(void* args){
	channel_t* chan = (channel_t *) args;
	void* data = NULL; 
	int ret = channel_recv(chan, &data);
	if(ret < 0){
		LOG_WRN("Failed to recieve data");
		return NULL;
	}
	int* value = (int*) data;
	LOG_INF("Recieved: %d", *value);
	free(value);
	return NULL;

}


int main() {
	channel_t* chan = channel_init(10);
	if(chan == NULL){
		LOG_ERR("Could not initialize channel");
		return -1;
	}

	LOG_DBG("Channel has been created, dispatching threads");
	// Launch threads
	pthread_t threads[THREADS];
	for (int i = 0; i < THREADS; ++i) {
		pthread_create(&threads[i], NULL, worker, chan);
	}
	
	LOG_DBG("Threads dispatched sending data down channel");
	for(int i = 0; i < 7; i ++){
		int* test = malloc(sizeof(int));
		*test = i;
		int ret = channel_send(chan, (void *) test);
		if(ret < 0){
			LOG_WRN("Failed to send data over channel");
		}
	}	

	LOG_DBG("Data sent waiting on threads");
	// Wait for all threads to complete
	for (int i = 0; i < THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}


	channel_dispose(chan);

	
	return 0;
}

