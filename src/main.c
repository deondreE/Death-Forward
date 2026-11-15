#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "logger.h"
#include "timer.h"
#include "util.h"
#include "arena.h"
#include "file_io.h"
#include "gfx.h"
#include "mdash.h"
#include "input.h"

static InputState current_keys; 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    input_set_key(&current_keys, key, true);
  } else if (action == GLFW_RELEASE) {
    input_set_key(&current_keys, key, false);
  }
}

int main(void) {
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not be intialized GLFW\n");
    return EXIT_FAILURE;
  }

  if (!logger_init("engine.log")) return -1;
  timer_init();
  
  LOG_I("Test INFO LOG");
  LOG_W("TEST WARN LOG");
  LOG_E("TEST ERROR LOG");

  Arena arena;
  arena_init(&arena, 1024*1024);

  StringView vert = file_read_to_arena(&arena, "assets/shaders/default.vert");
  StringView frag = file_read_to_arena(&arena, "assets/shaders/default.frag");
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: failed to create GLFW window!\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "ERROR: Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }
  glfwSetKeyCallback(window, key_callback);
  
  gfx_init(vert, frag);

  float aspect = 800.0f / 600.0f;
  Mat4 projection = mat4_perspective(1.047f, aspect, 0.1f, 100.0f);
  
  while(!glfwWindowShouldClose(window)) {
    timer_update();

    float dt = timer_delta();
    float time = glfwGetTime();

    Mat4 rot = mat4_rotate_z(time);
    Mat4 pos = mat4_translate((Vec3){0.0f, 0.0f, -2.0f});
    Mat4 model = mat4_mul(pos, rot);

    gfx_set_mat4("u_Projection", projection);
    gfx_set_mat4("u_Model", model);
    
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    gfx_set_vec4("u_Color", 0.4f, 0.5f, 0.8f, 1.0f);
    
    gfx_draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  gfx_shutdown();
  arena_destroy(&arena);
  logger_shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
