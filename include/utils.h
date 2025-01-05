#pragma once
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", file_path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    printf("File with shaders has been read\n");
    return buffer;
}

GLuint compile_shader(const char* shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Failed to compile shader:\n%s\n", info_log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint create_shader_program(const char* vertexPath, const char* fragmentPath) {
    char* vertexCode = read_file(vertexPath);
    char* fragmentCode = read_file(fragmentPath);

    if (!vertexCode || !fragmentCode) {
        fprintf(stderr, "Failed to read shader files\n");
        free(vertexCode);
        free(fragmentCode);
        return 0;
    }

    GLuint vertexShader = compile_shader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentCode, GL_FRAGMENT_SHADER);

    free(vertexCode);
    free(fragmentCode);

    if (!vertexShader || !fragmentShader) {
        fprintf(stderr, "Failed to compile shaders\n");
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
        fprintf(stderr, "Failed to link shader program:\n%s\n", info_log);
        glDeleteProgram(shaderProgram);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif // UTILS_H_
