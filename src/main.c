#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

int main(void) {
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not be intialized GLFW\n");
    return EXIT_FAILURE;
  }

  if (!logger_init("engine.log")) return -1;

  LOG_I("Test INFO LOG");
  LOG_W("TEST WARN LOG");
  LOG_E("TEST ERROR LOG");
  
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
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  logger_shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
