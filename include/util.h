#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "logger.h"

#ifdef Debug
#define E_ASSERT(expr, msg) \
  do { \
    if (!(expr)) {                                    \
      LOG_E("ASSERTION FAILED: %s | %s", #expr, msg); \
      __debugbreak(); \
  }\
} while(0)
#else
#define E_ASSERT(expr, msg) (void(0)) 
#endif

typedef struct {
  const char* data;
  size_t len;
} StringView;

#define SV(str) ((StringView){.data = (str), .len = sizeof(str) - 1})
#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).len, (sv).data

StringView sv_from_parts(const char* data, size_t len);

StringView sv_from_cstr(const char* cstr);

bool sv_eq(StringView a, StringView b);

StringView sv_slice(StringView sv, size_t start, size_t len);
StringView sv_trim_left(StringView sv);

#endif // UTIL_H
