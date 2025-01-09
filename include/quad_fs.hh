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
