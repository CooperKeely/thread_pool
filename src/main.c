#define LOG_LEVEL 3
#define LOG_MODULE_NAME "MAIN"
#include "../include/log.h"

#include "../include/arc_mutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 4
#define INCREMENTS 10000

// Destructor for the shared integer
void int_destructor(void* ptr) {
    int* val = ptr;
    LOG_DBG("Destructor called! Final value = %d\n", *val);
    free(val);
}

// Worker thread: clone and increment the shared counter
void* worker(void* arg) {
    
	arc_mutex_t* local = arc_mutex_clone((arc_mutex_t*)arg);

	for (int i = 0; i < INCREMENTS; ++i) {
		arc_mutex_lock(local);
		int* val = arc_mutex_get(local);
		(*val)++;
		arc_mutex_unlock(local);
	}
	
	arc_mutex_drop(local);

	return NULL; // local is auto-dropped
}

int main() {
	// Dynamically allocate the counter and initialize
	int* counter = malloc(sizeof(int));
	*counter = 0;

	// Use auto-drop for the main reference
	arc_mutex_t* shared = arc_mutex_init(counter, int_destructor);

	// Launch threads
	pthread_t threads[THREADS];
	for (int i = 0; i < THREADS; ++i) {
		pthread_create(&threads[i], NULL, worker, shared);
	}

	// Wait for all threads to complete
	for (int i = 0; i < THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	// Safely read final value
	arc_mutex_lock(shared);
	int final = *(int*)arc_mutex_get(shared);
	arc_mutex_unlock(shared);

	LOG_INF("Final value (main): %d", final);

   	arc_mutex_drop(shared);
	
	return 0;
}

