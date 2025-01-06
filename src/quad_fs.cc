#include "../include/quad_fs.h"


// Initialize fullscreen quad.
QuadFS* quad_fs() {
  float vertices[] = {
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,

    -1.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
    -1.0f, -1.0f,  0.0f
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  QuadFS* quad = (QuadFS*)malloc(sizeof(QuadFS));
  quad->VBO = VBO;
  quad->VAO = VAO;
  quad->vertices = vertices;
  return quad;
}

void destroy_quadfs(QuadFS* quad) {
  glDeleteBuffers(1, &quad->VBO);
  glDeleteVertexArrays(1, &quad->VAO);
  free(quad);
}
