#include "../include/arena.h"

arena_t* arena_init(size_t size){
	arena_t* arena = malloc(sizeof(arena_t));
	assert(arena != NULL);

	arena->capacity =  (size < DEFAULT_ARENA_SIZE) ? DEFAULT_ARENA_SIZE : size; 

	arena->mem_ptr = malloc(arena->capacity);
	assert(arena->mem_ptr != NULL);
	
	arena->next_free = 0;
	return arena;
}

void arena_deinit(arena_t* arena){
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void* arena_allocate(arena_t* arena, size_t size){
	assert(arena && "Arena is null");
	if (arena_get_pos(arena) + size <= arena->capacity){
		void* ptr = arena->mem_ptr + arena->next_free;
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}	
	assert(false && "Arena Full");
	errno = ENOMEM;
	return NULL;
}

void* arena_deallocate(arena_t* arena, size_t size){
	assert(arena && "Arena is null");
	if (arena_get_pos(arena) - size > 0) {
		arena->next_free -= size;
		return arena->mem_ptr + arena->next_free;
	}	
	assert(false && "Tried to remove from an empty arena");
	errno = EINVAL;
	return NULL;		
}

size_t arena_get_pos(arena_t* arena){
	assert(arena && "Arena is null");
	return arena->next_free;
}


