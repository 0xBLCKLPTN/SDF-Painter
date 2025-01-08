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
    #include <GLFW/glfw3.h>
#else
    #include "include/GL/glew.h"
    #include <GLFW/glfw3.h>
#endif

#include "utils.hh"


typedef struct {
    GLuint shader_program;
    GLuint (*compile_shader)(const char*, GLenum);
} Shader;

Shader* create_shader_program(const char* vertex_path, const char* fragment_path);
GLuint compile_shader(const char* shader_source, GLenum shader_type, char* error_message);


#endif /* SHADER_HH_ */
