#ifndef QUAD_FS_HH
#define QUAD_FS_HH

/*
*   Fullscreen Quad (QuadFS) Component. 
* 
* FILENAME: quad_fs.h
*
*/



#if defined(__linux__) || defined(__APPLE__)
  #include <GL/glew.h>
#else
  #include "../include/GL/glew.h"
#endif

#include <GLFW/glfw3.h>

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
