#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

typedef struct {
	uint32_t id;
	void* Job;
	Reciever* reciever;
} Worker;

typedef struct {


} Sender_t;

typedef struct {


} Reciever_t;

typedef struct {
	uint32_t size;
	Worker* worker;
	Sender sender;
} ThreadPool_t;


bool new_thread_pool(uint32_t);
	




