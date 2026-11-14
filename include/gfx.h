
#ifndef GFX_H
#define GFX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "util.h"
#include "mdash.h"

bool gfx_init(StringView vert_src, StringView frag_src);

void gfx_draw(void);

void gfx_shutdown(void);

void gfx_set_float(const char* name, float value);
void gfx_set_mat4(const char* name, Mat4  matrix);
void gfx_set_vec4(const char* name, float x, float y, float z, float w);

#endif
