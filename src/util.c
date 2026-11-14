#include "util.h"
#include <string.h>

StringView sv_from_parts(const char* data, size_t len) {
  return (StringView){.data = data, .len = len};
}

StringView sv_from_cstr(const char* cstr) {
  return (StringView){.data = cstr, .len = strlen(cstr)};
}

bool sv_eq(StringView a, StringView b) {
  if (a.len != b.len) return false;
  return memcmp(a.data, b.data, a.len) == 0;
}

StringView sv_slice(StringView sv, size_t start, size_t len) {
  if (start >= len) return (StringView){NULL, 0}; // could technically report error here or something.
  if (start + len > sv.len) len = sv.len - start;
  return (StringView){.data = sv.data + start, .len = len};
} 

StringView sv_trim_left(StringView sv) {
  size_t i = 0;
  while (i < sv.len && (sv.data[i] == ' ' || sv.data[i] == '\t' || sv.data[i] == '\n')) {
    i++;
  }
  return sv_slice(sv, i, sv.len-i);
}

