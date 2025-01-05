/*
*   Fullscreen Quad (QuadFS) Component. 
* 
* FILENAME: quad_fs.h
*
*/


#ifndef QUAD_FS_H_
#define QUAD_FS_H_

#include <GL/glew.h>
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
