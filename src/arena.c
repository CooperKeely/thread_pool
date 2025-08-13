#include "../include/arena.h"

arena_t* ArenaAlloc(void){
	arena_t* arena = malloc(sizeof(arena_t));
	assert(arena && "memory allocation failed");

	arena->capacity = DEFAULT_ARENA_SIZE; 

	arena->mem_ptr = malloc(arena->capacity);
	assert(arena->mem_ptr && "memory allocation failed");
	
	arena->next_free = 0;
	return arena;
}

void ArenaRelease(arena_t* arena){
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void* ArenaPush(arena_t* arena, size_t size){
	assert(arena && "Arena is null");
	if (ArenaGetPos(arena) + size <= arena->capacity){
		void* ptr = arena->mem_ptr + arena->next_free;
		arena->next_free += size;
		return ptr;
	}	
	assert(false && "Arena Full");
	return NULL;
}



void* ArenaPushZero(arena_t* arena, size_t size){
	assert(arena && "Arena is null");
	if (ArenaGetPos(arena) + size <= arena->capacity){
		void* ptr = arena->mem_ptr + arena->next_free;
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}	
	assert(false && "Arena Full");
	return NULL;
}

void* ArenaPop(arena_t* arena, size_t size){
	assert(arena && "Arena is null");
	if (ArenaGetPos(arena) - size > 0) {
		arena->next_free -= size;
		return arena->mem_ptr + arena->next_free;
	}	
	assert(false && "Tried to remove from an empty arena");
	return NULL;		
}

size_t ArenaGetPos(arena_t* arena){
	assert(arena && "Arena is null");
	return arena->next_free;
}


