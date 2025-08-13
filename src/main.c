#define LOG_LEVEL 3
#include "../include/log.h"

#include <pthread.h>

#include "../include/arena.h"
#include "../include/thread_pool.h"

#define THREADS 100

void *worker(void *args) {
  printf("Hello!!\n");
  return args;
}

int main() {
  arena_t *arena = ArenaAlloc();
  LOG_DBG("Threads have been joined destorying channel");
  thread_pool_t *thread_pool = ThreadPoolInit(arena, 10);
  ThreadPoolDispatch(thread_pool, worker, NULL);

  ArenaRelease(arena);
  return 0;
}
