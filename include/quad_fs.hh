#ifndef QUAD_FS_HH
#define QUAD_FS_HH

/*
*   Fullscreen Quad (QuadFS) Component. 
* 
* FILENAME: quad_fs.h
*
*/




#if defined(__APPLE__) || defined(__linux__)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#else
    #include "GL/glew.h"
    #if defined(__x86_64__) || defined(_M_X64)
        #include "GLFW_x64/glfw3.h"
    #else
        #include "GLFW_x32/glfw3.h"
    #endif
#endif

#include <stdlib.h>


//  Fullscreen Quad struct that storage VBO, VBA (Vertex Buffers)
// and vertices of quad.
typedef struct {
  float* vertices;
  GLuint VAO, VBO;
} QuadFS;

QuadFS* quad_fs();

void destroy_quadfs(QuadFS* quad);
#endif // QUAD_FS_H_
