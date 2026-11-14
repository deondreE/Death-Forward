#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

void arena_init(Arena* arena, size_t capacity) {
  arena->buffer = (u8*)malloc(capacity);
  arena->capacity = capacity;
  arena->offset = 0;
  E_ASSERT(arena->buffer != NULL, "OS failed to allocate intial arena buffer");
}

void* arena_alloc(Arena* arena, size_t size) {
  E_ASSERT(arena->buffer != NULL, "Attempted to allocated from a null buffer.");
  size_t aligned_size = (size + 7) & ~7;

  E_ASSERT(arena->offset + aligned_size <= arena->capacity, "Arena capacity exceeded.");
  void* ptr = arena->buffer + arena->offset;
  arena->offset += aligned_size;

  memset(ptr, 0, size);

  return ptr;
}

size_t arena_checkpoint(Arena* arena) {
  return arena->offset;
}

void arena_rollback(Arena* arena, size_t checkpoint) {
  E_ASSERT(checkpoint <= arena->offset, "Cannot rollback to a future checkpoint");
  arena->offset = checkpoint;
}

void arena_reset(Arena* arena) {
  E_ASSERT(arena->buffer != NULL, "Attempted to reset a null buffer");
  arena->offset = 0;
}

void arena_destroy(Arena* arena) {
  if (arena->buffer) {
    free(arena->buffer);
    arena->buffer = NULL;
  }
  arena->capacity = 0;
  arena->offset = 0;
}
