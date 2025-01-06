#include "../include/callbacks.h"


// ===========================================================
//
// Mouse callbacks and framebuffer callback. Now we can do something with our mouse ( if i program it later).

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
#ifdef DEBUG
  printf("XPOS: %f - YPOS: %f\r", xpos, ypos);
#endif
  return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
#ifdef DEBUG
    printf("Right mouse button pressed\r");
#endif
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
#ifdef DEBUG
    printf("Right mouse button released\r");
#endif
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  return;
}



