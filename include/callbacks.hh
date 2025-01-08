//
//  callbacks.hh
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#ifndef CALLBACKS_HH
#define CALLBACKS_HH

#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif //CALLBACKS_HH


