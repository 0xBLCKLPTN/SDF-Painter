/*
*     Signed Distance Field (SDF) Object Engine.
*
* FILENAME: main.c
* DESCRIPTION: This code is a start point for engine.
*
*/
#include <GL/glew.h>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef ENABLE_OPENCL
#include <CL/cl.h>
#endif

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
  int window;
  GLuint default_shader_program;
  QuadFS* quad; // Add a pointer to the QuadFS struct
  double mouse_x, mouse_y;
  vec3 camera_position; // Добавьте позицию камеры
  vec3 camera_lookAt;   // Добавьте направление камеры
  int selected_object_id;

#ifdef ENABLE_OPENCL
  cl_context context;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;
#endif
} Application;

// ===========================================================
//
// Mouse callbacks and framebuffer callback. Now we can do something with our mouse ( if i program it later).

void mouse_callback(int x, int y) {
  printf("XPOS: %d - YPOS: %d\r", x, y);
  return;
}

void mouse_button_callback(int button, int state, int x, int y) {
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    printf("Right mouse button pressed\r");
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    printf("Right mouse button released\r");
  }
}

void scroll_callback(int button, int dir, int x, int y) {
  return;
}

// Resize our window.
void framebuffer_size_callback(int width, int height) {
  glViewport(0, 0, width, height); // TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
}

#ifdef ENABLE_OPENCL
void init_opencl(Application* application) {
  cl_int err;
  cl_platform_id platform;
  cl_device_id device;
  cl_uint num_platforms, num_devices;

  // Get platform and device information
  err = clGetPlatformIDs(1, &platform, &num_platforms);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to get platform IDs\n");
    return;
  }

  err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to get device IDs\n");
    return;
  }

  // Create an OpenCL context
  application->context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to create OpenCL context\n");
    return;
  }

  // Create a command queue
  application->queue = clCreateCommandQueue(application->context, device, 0, &err);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to create command queue\n");
    return;
  }

  // Load and build the OpenCL program
  const char* source = "__kernel void sample_kernel(__global const float* input, __global float* output) { ... }";
  application->program = clCreateProgramWithSource(application->context, 1, &source, NULL, &err);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to create OpenCL program\n");
    return;
  }

  err = clBuildProgram(application->program, 1, &device, NULL, NULL, NULL);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to build OpenCL program\n");
    return;
  }

  // Create the OpenCL kernel
  application->kernel = clCreateKernel(application->program, "sample_kernel", &err);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to create OpenCL kernel\n");
    return;
  }
}

void destroy_opencl(Application* application) {
  clReleaseKernel(application->kernel);
  clReleaseProgram(application->program);
  clReleaseCommandQueue(application->queue);
  clReleaseContext(application->context);
}
#endif

Application* init_application(uint32_t width, uint32_t height, char* name) {
  int argc = 1;
  char* argv[1] = { (char*)"SDFE" };
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);

  Application* application = (Application*)malloc(sizeof(Application));
  application->window = glutCreateWindow(name);

  glutDisplayFunc(display_callback);
  glutReshapeFunc(framebuffer_size_callback);
  glutPassiveMotionFunc(mouse_callback);
  glutMouseFunc(mouse_button_callback);
  glutMotionFunc(mouse_button_callback);

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

#ifdef ENABLE_OPENCL
  init_opencl(application);
#endif

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
    if (glutGetModifiers() & GLUT_ACTIVE_ALT && glutGetModifiers() & GLUT_ACTIVE_CTRL) {
        glutLeaveMainLoop();
    }

    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        int x = glutGet(GLUT_WINDOW_X) + glutGet(GLUT_WINDOW_WIDTH) / 2;
        int y = glutGet(GLUT_WINDOW_Y) + glutGet(GLUT_WINDOW_HEIGHT) / 2;
        glutWarpPointer(x, y);
    }

    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        glutGetCursorPos(application->mouse_x, application->mouse_y);
    }

    if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
      glutGetCursorPos(application->mouse_x, application->mouse_y);
      printf("Left mouse button pressed at (%f, %f)\n", application->mouse_x, application->mouse_y);
      // Set the selected object ID to -1 to indicate a selection is being made
      application->selected_object_id = -1;
    }

    // Calculate the forward direction
    vec3 forward = vec3_normalize(vec3_sub(application->camera_lookAt, application->camera_position));

    // Movement speed
    float speed = 0.1f;

    // Handle camera movement
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT && glutGetModifiers() & GLUT_KEY_UP) {
        application->camera_position = vec3_add(application->camera_position, vec3_mul(forward, speed));
    }
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT && glutGetModifiers() & GLUT_KEY_DOWN) {
        application->camera_position = vec3_sub(application->camera_position, vec3_mul(forward, speed));
    }
}

void destroy_application(Application* application) {
  glDeleteProgram(application->default_shader_program);
  destroy_quadfs(application->quad);
  // Do not free application->window_title as it is a string literal
  free(application);

#ifdef ENABLE_OPENCL
  destroy_opencl(application);
#endif
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

void display_callback() {
  event_handler(application);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  draw(application);
  glutSwapBuffers();
  glutPostRedisplay();
}

void run_application(Application* application) {
  prepare_to_render(application);
  glutMainLoop();
  destroy_application(application);
  return;
}

int main(int argc, char* argv[]) {
  Application* application = init_application(800, 600, "SDFE");
  run_application(application);
  return 0;
}
