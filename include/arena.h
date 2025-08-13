#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

/* default arena size is much larger then needed */
/* to avoid needing to resize the arena */
#define DEFAULT_ARENA_SIZE 64 * 1024 

typedef struct {
	void* mem_ptr;
	size_t next_free;
	size_t capacity;
} arena_t;

/* allocate and free arena */
arena_t* ArenaAlloc(void);
void ArenaRelease(arena_t* arena); 

/* allocate memory from the arena */
void* ArenaPush(arena_t* arena, size_t size);
void* ArenaPushZero(arena_t* arena, size_t size);

/* helper macros */
#define PushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type)*(count))
#define PushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type)*(count))
#define PushStruct(arena, type) PushArray((arena), type, (1))
#define PushStructZero(arena, type) PushArrayZero((arena), type, (1))

/* free memory from the arena */
void* ArenaPop(arena_t* arena, size_t size);
void ArenaClear(arena_t* arena);

/* helper functions */
size_t ArenaGetPos(arena_t* arena);
void ArenaSetPosBack(arena_t* arena, size_t size);


#endif /*ARENA_H*/
