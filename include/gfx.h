#ifndef GFX_H
#define GFX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "util.h"

bool gfx_init(StringView vert_src, StringView frag_src);

void gfx_draw(void);

void gfx_shutdown(void);

#endif
