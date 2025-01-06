#include "../include/callbacks.h"


// ===========================================================
//
// Mouse callbacks and framebuffer callback. Now we can do something with our mouse ( if i program it later).

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  printf("XPOS: %f - YPOS: %f\r", xpos, ypos);
  return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    printf("Right mouse button pressed\r");
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    printf("Right mouse button released\r");
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  return;
}

// Resize our window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height); // TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
}
