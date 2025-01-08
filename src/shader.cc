//
//  shader.cpp
//  sdfe
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#include "../include/shader.hh"

GLuint compile_shader(const char* shader_source, GLenum shader_type, char* error_message) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, error_message);
#if (LOG_LEVEL == 1)
    fprintf(stderr, "Failed to compile shader:\n%s\n", error_message);
#endif
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

Shader* create_shader_program(const char* vertex_path, const char* fragment_path) {
  Shader* shader = (Shader*)malloc(sizeof(Shader));
  if (!shader) {
    fprintf(stderr, "Cannot allocate memory for Shader struct\n");
    return NULL;
  }

  char* vertex_code = read_file(vertex_path);
  char* fragment_code = read_file(fragment_path);
  if (!vertex_code || !fragment_code) {
    fprintf(stderr, "Failed to read shader files\n");
    free(vertex_code);
    free(fragment_code);
    free(shader);
    return NULL;
  }

  GLuint vertex_shader = 0;
  GLuint fragment_shader = 0;
  char error_message[512];
  int retry_count = 3; // Number of retries

  while (retry_count > 0) {
    vertex_shader = compile_shader(vertex_code, GL_VERTEX_SHADER, error_message);
    fragment_shader = compile_shader(fragment_code, GL_FRAGMENT_SHADER, error_message);

    if (vertex_shader && fragment_shader) {
      break;
    }

    retry_count--;
    if (retry_count > 0) {
#ifdef DEBUG
      fprintf(stderr, "Retrying to compile shaders...\n");
#endif
    }
  }

  free(vertex_code);
  free(fragment_code);

  if (!vertex_shader || !fragment_shader) {
#ifdef DEBUG
    fprintf(stderr, "Failed to compile shaders after retries\n");
#endif
    free(shader);
    return NULL;
  }

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  GLint success;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, error_message);
#ifdef DEBUG
    fprintf(stderr, "Failed to link shader program:\n%s\n", error_message);
#endif
    glDeleteProgram(shader_program);
    free(shader);
    return NULL;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  shader->shader_program = shader_program;
  return shader;
}
