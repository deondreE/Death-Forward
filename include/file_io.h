#ifndef FILE_IO_H
#define FILE_IO_H

#include "arena.h"
#include "util.h"

StringView file_read_to_arena(Arena* arena, const char* path);

#endif
