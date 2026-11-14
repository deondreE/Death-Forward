#include "file_io.h"
#include <stdio.h>
#include "logger.h"

StringView file_read_to_arena(Arena* arena, const char* path) {
  FILE* fp = NULL;
  fopen_s(&fp, path, "rb");
  if (!fp) {
    LOG_E("Failed to open file: %s", path);
    return (StringView){NULL, 0};
  }

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);
  if (size <= 0) {
    LOG_W("File is empty or could not determine size: %s", path);
    fclose(fp);
    return (StringView){NULL, 0};
  }

  char* buffer = (char*)arena_alloc(arena, (size_t)size + 1);

  size_t read_bytes = fread(buffer, 1, (size_t)size, fp);
  fclose(fp);

  if(read_bytes != (size_t)size) {
    LOG_E("Failed to read full file content: %s", path);
    return (StringView){NULL, 0};
  }

  buffer[size] = '\0';

  return (StringView){.data = buffer, .len = (size_t)size};
}
