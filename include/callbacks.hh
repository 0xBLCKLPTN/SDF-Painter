//
//  callbacks.hh
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#ifndef CALLBACKS_HH
#define CALLBACKS_HH

#if defined(__APPLE__) || defined(__linux__)
    #include <GL/glew.h>
#elif defined(_W32)
    #include "GL/glew.h"
#endif

#if defined(__x86_64__) || defined(_M_X64)
    #include "GLFW/x64/glfw3.h"
    #include "GLFW/x64/glfw3native.h"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #include "GLFW/x32/glfw3.h"
    #include "GLFW/x32/glfw3native.h"
#else
    #define PLATFROM_DOEST_SUPPORTED
#endif

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif //CALLBACKS_HH


