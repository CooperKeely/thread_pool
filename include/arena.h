#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

// default arena size is much larger then needed
// to avoid needing to resize the arena
#define DEFAULT_ARENA_SIZE 8096

typedef struct {
	void* mem_ptr;
	size_t next_free;
	size_t capacity;
} arena_t;

arena_t* arena_init(size_t size);
void arena_deinit(arena_t* arena); 

void* arena_allocate(arena_t* arena, size_t size);
void* arena_deallocate(arena_t* arena, size_t size);
size_t arena_get_pos(arena_t* arena);

#endif // ARENA_H
