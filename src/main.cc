//
//     Signed Distance Field (SDF) Object Engine.
//
// FILENAME: main.c
//  DESCRIPTION: This code is a start point for engine.
//
//
// ================== SDF Painter Engine Flags ============
//    Define ENABLE_GUI via `#define ENABLE_GUI` if you want to enable Engine GUI.
// this flag enables also enables all IMGUI functional.
//
// Define DEBUG via `#define DEBUG` if you want to see printf() result in your stdout.
// Define ENABLE_VSYNC `#define ENABLE_VSYNC` if you want vertical syncronization.
// Define EXP_SHADER_ST `#define EXP_SHADER_ST` if you want more OOP on shader.
#define ENABLE_GUI
#define DEBUG
#define ENABLE_VSYNC
#define EXP_SHADER_ST
// ========================================================

// ================== INCLUDE IMGUI LIBRARIES =============
//    If ENABLE GUI flag is defined, we include imgui headers file and
// imgui backends. Now it is a GLFW support and OpenGL. Also, if we want do silence OpenGL,
// we use `#define GL_SILENCE_DEPRECATION` flag.
#if defined(ENABLE_GUI)
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#endif // ENABLE_GUI END.

// =================== GLEW AND GLFW =====================
//  GLEW ( OpenGL Extensios Wrangler Library ) - https://glew.sourceforge.net/
//  GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan
// development on the desktop. It provides a simple API for creating windows, contexts and
// surfaces, receiving input and events. - https://www.glfw.org/
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
#include "../include/callbacks.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if defined(ENABLE_GUI)
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#endif

// Define some constants, like a OpenGL minor and major versions.
#define CONTEXT_VERSION_MAJOR 3
#define CONTEXT_VERSION_MINOR 3
#define RESOURCES_PATH "resources/"

#ifdef EXP_SHADER_ST
typedef struct {
  GLuint shader_program;
  GLuint (*compile_shader_exp)(const char*, GLenum);
} Shader;

GLuint compile_shader_exp(const char* shader_source, GLenum shader_type, char* error_message) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, error_message);
#ifdef DEBUG
    fprintf(stderr, "Failed to compile shader:\n%s\n", error_message);
#endif
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

Shader* create_shader_program_exp(const char* vertex_path, const char* fragment_path) {
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
    vertex_shader = compile_shader_exp(vertex_code, GL_VERTEX_SHADER, error_message);
    fragment_shader = compile_shader_exp(fragment_code, GL_FRAGMENT_SHADER, error_message);

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


#endif // EXP_SHADER_ST


//    C struct (o-yeea. this is a class in C language) for store some
// application data, like a window sizes or something else.
typedef struct {
  vec2_uint32_t sizes;
  int width, height;
  char* window_title;
  GLFWwindow* window;
#ifdef EXP_SHADER_ST
  Shader* default_shader_program;
#else
  GLuint default_shader_program;
#endif  
  QuadFS* quad; // Add a pointer to the QuadFS struct
  double mouse_x, mouse_y;
  vec3 camera_position;
  vec3 camera_lookAt;
  int selected_object_id;

#ifdef ENABLE_GUI
  bool enable_gui;
#endif

} Application;

// Resize our window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
#ifdef DEBUG
  printf("FSC: %d-%d\n", width, height);
#endif
  glViewport(0, 0, width, height); // TODO: In HDPI can be errors. I try in on MacBook 11.1. And have errors...
}


Application* init_application(int width, int height, char* name) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return NULL;
  }
  IMGUI_CHECKVERSION();
  
#if __APPLE__
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 330";
#else
  // GL 3.2 + GLSL 130
  const char *glsl_version = "#version 330";
#endif
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

#ifdef ENABLE_VSYNC
  glfwSwapInterval(1);
#endif


  glfwSetFramebufferSizeCallback(application->window, framebuffer_size_callback);
  glfwSetCursorPosCallback(application->window, mouse_callback);
  glfwSetScrollCallback(application->window, scroll_callback);
  glfwSetMouseButtonCallback(application->window, mouse_button_callback);

#ifdef ENABLE_GUI
  // Setup CIMGUI
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
 
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls.

#ifdef IMGUI_HASH_DOCK
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking.
  io.COnfigFlags |= ImGuiConfigFlags_ViewportsEnable // Enable Multi-Viewport / Platform Winwdows;
#endif

  ImGui::StyleColorsDark();
  
  ImGui_ImplGlfw_InitForOpenGL(application->window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
#endif

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW.\n");
    free(application);
    return NULL;
  }

  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glViewport(0, 0, width, height);
  

  application->width = width;
  application->height = height;
  application->window_title = name;
  application->camera_position = (vec3){-20.0f, 0.0f, -3.0f}; // Инициализируйте позицию камеры
  application->camera_lookAt = (vec3){20.0f, 0.0f, 0.0f};     // Инициализируйте направление камеры
  application->selected_object_id = -1;

#ifdef ENABLE_GUI
  application->enable_gui = true;
#else
  application->enable_gui = false;
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

#ifdef EXP_SHADER_ST
    application->default_shader_program = create_shader_program_exp(vertexShaderPath, fragmentShaderPath);
    if (!application->default_shader_program || !application->default_shader_program->shader_program) {
        fprintf(stderr, "Failed to create shader program.\n");
        free(application);
    }
#else
    application->default_shader_program = create_shader_program(vertexShaderPath, fragmentShaderPath);
    if (!application->default_shader_program) {
        fprintf(stderr, "Failed to create shader program.\n");
        free(application);
    }
#endif
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
#ifdef DEBUG
      printf("Left mouse button pressed at (%f, %f)\n", application->mouse_x, application->mouse_y);
#endif
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
#ifdef EXP_SHADER_ST
  glDeleteProgram(application->default_shader_program->shader_program);
#else
  glDeleteProgram(application->default_shader_program);
#endif
  destroy_quadfs(application->quad);
  // Do not free application->window_title as it is a string literal
  free(application);
  glfwTerminate();
}


void draw(Application* application) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(application->window, &fbWidth, &fbHeight);

#ifdef EXP_SHADER_ST
    glUseProgram(application->default_shader_program->shader_program);
    GLint resolution_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_resolution");
    glUniform2f(resolution_location, fbWidth, fbHeight);

    GLuint mouse_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_mouse");
    glUniform2f(mouse_location, application->mouse_x, application->mouse_y);

    GLuint camera_position_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_camera_position");
    glUniform3f(camera_position_location, application->camera_position.x, application->camera_position.y, application->camera_position.z);

    GLuint camera_lookat_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_camera_lookAt");
    glUniform3f(camera_lookat_location, application->camera_lookAt.x, application->camera_lookAt.y, application->camera_lookAt.z);

    GLuint selected_object_id_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_selected_object_id");
    glUniform1i(selected_object_id_location, application->selected_object_id);

    // Set the u_time uniform
    GLuint time_location = glGetUniformLocation(application->default_shader_program->shader_program, "u_time");
    float time = glfwGetTime();
    glUniform1f(time_location, time);

#else
    glUseProgram(application->default_shader_program);
    GLint resolutionLocation = glGetUniformLocation(application->default_shader_program, "u_resolution");
    glUniform2f(resolutionLocation, fbWidth, fbHeight);

    GLuint mouseLocation = glGetUniformLocation(application->default_shader_program, "u_mouse");
    glUniform2f(mouseLocation, application->mouse_x, application->mouse_y);

    GLuint cameraPositionLocation = glGetUniformLocation(application->default_shader_program, "u_camera_position");
    glUniform3f(cameraPositionLocation, application->camera_position.x, application->camera_position.y, application->camera_position.z);

    GLuint cameraLookAtLocation = glGetUniformLocation(application->default_shader_program, "u_camera_lookAt");
    glUniform3f(cameraLookAtLocation, application->camera_lookAt.x, application->camera_lookAt.y, application->camera_lookAt.z);

    GLuint selectedObjectIdLocation = glGetUniformLocation(application->default_shader_program, "u_selected_object_id");
    glUniform1i(selectedObjectIdLocation, application->selected_object_id);

    // Set the u_time uniform
    GLuint timeLocation = glGetUniformLocation(application->default_shader_program, "u_time");
    float time = glfwGetTime();
    glUniform1f(timeLocation, time);
#endif

    glBindVertexArray(application->quad->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void run_application(Application* application) {
    prepare_to_render(application);

    time_t last_mod_time = get_file_modification_time("resources/fragment_shader.glsl");

    while (!glfwWindowShouldClose(application->window)) {
        glfwPollEvents();
#ifdef DEBUG
        printf("SCREEN SIZE: %i - %i\n", application->width, application->height);
#endif
        if (glfwGetWindowAttrib(application->window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
#ifdef ENABLE_GUI
      
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        // Show IMGUI demo

        // Create the menu bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { application->enable_gui = false; }
                if (ImGui::MenuItem("Exit", "Ctrl+Q")) { glfwSetWindowShouldClose(application->window, GLFW_TRUE); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do stuff */ }
                if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) { /* Do stuff */ }  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Do stuff */ }
                if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Do stuff */ }
                if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Do stuff */ }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Render();
#endif
        event_handler(application);

        // Check for shader file changes
        time_t current_mod_time = get_file_modification_time("resources/fragment_shader.glsl");
        if (current_mod_time != last_mod_time) {
            last_mod_time = current_mod_time;
#ifdef EXP_SHADER_ST
            glDeleteProgram(application->default_shader_program->shader_program);
#else
            glDeleteProgram(application->default_shader_program);
#endif
            prepare_to_render(application);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        draw(application);
#ifdef ENABLE_GUI
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
        glfwSwapBuffers(application->window);
    }
#ifdef ENABLE_GUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
#endif
    ImGui::DestroyContext();

    destroy_application(application);
    return;
}


int main(int argc, char* argv[]) {
  Application* application = init_application(800, 600, "SDFE");
  run_application(application);
  return 0;
}
