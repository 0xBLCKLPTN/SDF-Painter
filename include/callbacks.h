#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <stdio.h>
#include <GLFW/glfw3.h>


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Resize our window.
// TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);


#endif // CALLBACKS_H_
