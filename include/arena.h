#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef struct {
  u8* buffer;
  size_t capacity;
  size_t offset;
} Arena;

void arena_init(Arena* arena, size_t capacity);

void* arena_alloc(Arena* arena, size_t size);

size_t arena_checkpoint(Arena* arena);

void arena_rollback(Arena* arena, size_t checkpoint);

void arena_reset(Arena* arena);

void arena_destroy(Arena* arena);

#endif
