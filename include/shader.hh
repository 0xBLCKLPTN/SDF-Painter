//
//  shader.hpp
//  sdfe
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#ifndef SHADER_HH_
#define SHADER_HH_


#include <stdio.h>
#include <stdlib.h>

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


#include "utils.hh"


typedef struct {
    GLuint shader_program;
    GLuint (*compile_shader)(const char*, GLenum);
} Shader;

Shader* create_shader_program(const char* vertex_path, const char* fragment_path);
GLuint compile_shader(const char* shader_source, GLenum shader_type, char* error_message);


#endif /* SHADER_HH_ */
