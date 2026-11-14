#include "gfx.h"
#include "logger.h"

static GLuint shader_program;
static GLuint vao, vbo;

static GLuint compile_shader(GLenum type, StringView src) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src.data, (GLint*)&src.len);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info[512];
    glGetShaderInfoLog(shader, 512, NULL, info);
    LOG_E("Shader Compile Error: %s", info);
  }
  return shader;
}

bool gfx_init(StringView vert_src, StringView frag_src) {
  GLuint vs = compile_shader(GL_VERTEX_SHADER, vert_src);
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, frag_src);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  return true;
}

void gfx_draw(void) {
  glUseProgram(shader_program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void gfx_shutdown(void) {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shader_program);
}
