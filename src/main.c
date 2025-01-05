/*
*     Signed Distance Field (SDF) Object Engine.
*
* FILENAME: main.c
* DESCRIPTION: This code is a start point for engine.
*
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "../include/modificated_math.h"
#include "../include/utils.h"
#include "../include/components.h"

// Define some constants, like a OpenGL minor and major versions.
#define CONTEXT_VERSION_MAJOR 3
#define CONTEXT_VERSION_MINOR 3
#define RESOURCES_PATH "resources/"

//    C struct (o-yeea. this is a class in C language) for store some
// application data, like a window sizes or something else.
typedef struct {
  vec2_uint32_t sizes;
  uint32_t width, height;
  char* window_title;
  GLFWwindow* window;
  GLuint default_shader_program;
  QuadFS* quad; // Add a pointer to the QuadFS struct
  double mouse_x, mouse_y;
  vec3 camera_position; // Добавьте позицию камеры
  vec3 camera_lookAt;   // Добавьте направление камеры
  int selected_object_id;

} Application;

// ===========================================================
//
// Mouse callbacks and framebuffer callback. Now we can do something with our mouse ( if i program it later).

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  printf("XPOS: %f - YPOS: %f\r", xpos, ypos);
  return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    printf("Right mouse button pressed\r");
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    printf("Right mouse button released\r");
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  return;
}

// Resize our window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height); // TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
}

Application* init_application(uint32_t width, uint32_t height, char* name) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return NULL;
  }

  // Set OPENGL VERSION.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Application* application = (Application*)malloc(sizeof(Application));

  application->window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (!application->window) {
    fprintf(stderr, "Failed to create GLFW window.\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(application->window);

  glfwSetFramebufferSizeCallback(application->window, framebuffer_size_callback);
  glfwSetCursorPosCallback(application->window, mouse_callback);
  glfwSetScrollCallback(application->window, scroll_callback);
  glfwSetMouseButtonCallback(application->window, mouse_button_callback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW.\n");
    free(application);
    return NULL;
  }

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  application->width = width;
  application->height = height;
  application->window_title = name;
  application->camera_position = (vec3){-20.0f, 0.0f, -3.0f}; // Инициализируйте позицию камеры
  application->camera_lookAt = (vec3){20.0f, 0.0f, 0.0f};     // Инициализируйте направление камеры
  application->selected_object_id = -1;

  return application;
}

void prepare_to_render(Application* application) {
    // Allocate enough memory to hold the concatenated strings
    char vertexShaderPath[strlen(RESOURCES_PATH) + strlen("vertex_shader.glsl") + 1];
    char fragmentShaderPath[strlen(RESOURCES_PATH) + strlen("fragment_shader.glsl") + 1];

    // Use sprintf to concatenate the strings
    sprintf(vertexShaderPath, "%s%s", RESOURCES_PATH, "vertex_shader.glsl");
    sprintf(fragmentShaderPath, "%s%s", RESOURCES_PATH, "fragment_shader.glsl");

    application->default_shader_program = create_shader_program(vertexShaderPath, fragmentShaderPath);
    if (!application->default_shader_program) {
        fprintf(stderr, "Failed to create shader program.\n");
        free(application);
    }
    application->quad = quad_fs();
}

void event_handler(Application* application) {
    if (glfwGetKey(application->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(application->window, true);

    if (glfwGetMouseButton(application->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwGetCursorPos(application->window, &application->mouse_x, &application->mouse_y);
    }

    if (glfwGetMouseButton(application->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      glfwGetCursorPos(application->window, &application->mouse_x, &application->mouse_y);
      printf("Left mouse button pressed at (%f, %f)\n", application->mouse_x, application->mouse_y);
      // Set the selected object ID to -1 to indicate a selection is being made
      application->selected_object_id = -1;
    }

    // Calculate the forward direction
    vec3 forward = vec3_normalize(vec3_sub(application->camera_lookAt, application->camera_position));

    // Movement speed
    float speed = 0.1f;

    // Handle camera movement
    if (glfwGetKey(application->window, GLFW_KEY_UP) == GLFW_PRESS) {
        application->camera_position = vec3_add(application->camera_position, vec3_mul(forward, speed));
    }
    if (glfwGetKey(application->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        application->camera_position = vec3_sub(application->camera_position, vec3_mul(forward, speed));
    }
}

void destroy_application(Application* application) {
  glDeleteProgram(application->default_shader_program);
  destroy_quadfs(application->quad);
  // Do not free application->window_title as it is a string literal
  free(application);
  glfwTerminate();

}

void draw(Application* application) {
  glUseProgram(application->default_shader_program);
  GLint resolutionLocation = glGetUniformLocation(application->default_shader_program, "u_resolution");
  glUniform2f(resolutionLocation, application->width, application->height);

  GLuint mouseLocation = glGetUniformLocation(application->default_shader_program, "u_mouse");
  glUniform2f(mouseLocation, application->mouse_x, application->mouse_y);

  GLuint cameraPositionLocation = glGetUniformLocation(application->default_shader_program, "u_camera_position");
  glUniform3f(cameraPositionLocation, application->camera_position.x, application->camera_position.y, application->camera_position.z);

  GLuint cameraLookAtLocation = glGetUniformLocation(application->default_shader_program, "u_camera_lookAt");
  glUniform3f(cameraLookAtLocation, application->camera_lookAt.x, application->camera_lookAt.y, application->camera_lookAt.z);

  GLuint selectedObjectIdLocation = glGetUniformLocation(application->default_shader_program, "u_selected_object_id");
  glUniform1i(selectedObjectIdLocation, application->selected_object_id);

  glBindVertexArray(application->quad->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void run_application(Application* application) {
  prepare_to_render(application);
  while (!glfwWindowShouldClose(application->window))
  {
    event_handler(application);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    draw(application);
    glfwSwapBuffers(application->window);
    glfwPollEvents();
  }
  destroy_application(application);
  return;
}

int main(int argc, char* argv[]) {
  Application* application = init_application(800, 600, "SDFE");
  run_application(application);
  return 0;
}
