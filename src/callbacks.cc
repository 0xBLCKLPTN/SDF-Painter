//
//  callbacks.cc
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#include "../include/callbacks.hh"

// ===========================================================
//
// Mouse callbacks and framebuffer callback. Now we can do something with our mouse ( if i program it later).

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
#ifdef DEBUG
  printf("XPOS: %f - YPOS: %f\r", xpos, ypos);
#endif
  return;
}

// Resize our window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
#if (LOG_LEVEL == 1)
  printf("FSC: %d-%d\n", width, height);
#endif
  glViewport(0, 0, width, height); // TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
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
