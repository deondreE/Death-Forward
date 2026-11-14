#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "timer.h"
#include "util.h"
#include "arena.h"
#include "file_io.h"

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

  StringView vert_src = file_read_to_arena(&arena, "assets/shaders/default.vert");
  StringView frag_src = file_read_to_arena(&arena, "assets/shaders/default.frag");
  
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

  while(!glfwWindowShouldClose(window)) {
    timer_update();

    float dt = timer_delta();
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  arena_destroy(&arena);
  logger_shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
