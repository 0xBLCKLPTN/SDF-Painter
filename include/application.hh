#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define ENABLE_ENGINE_GUI
#define ENABLE_VSYNC
#define PRINT_LOGS
#define IMGUI_HASH_DOCK

#if defined(PRINT_LOGS)
    #define LOG_LEVEL 1
#elif defined(PRINT_ONLY_ERRORS)
    #define LOG_LEVEL 2
#else
    #define LOG_LEVEL 3
#endif

#if defined(__APPLE__) || defined(__linux__)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <unistd.h>
#elif defined(WIN32)
    #include "GL/glew.h"
    #include <GLFW/glfw3.h>
#endif

// ================== INCLUDE IMGUI LIBRARIES =============
#if defined(ENABLE_ENGINE_GUI)
    #include "imgui/imgui.h"
    #include "imgui/imgui_impl_glfw.h"
    #include "imgui/imgui_impl_opengl3.h"

    #define GL_SILENCE_DEPRECATION
        #if defined(IMGUI_IMPL_OPENGL_ES2)
        #include <GLES2/gl2.h>
    #endif
#endif // ENABLE_GUI END.

#if defined(ENABLE_ENGINE_GUI)
    #if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
    #pragma comment(lib, "legacy_stdio_definitions")
#endif
#endif

#define CONTEXT_VERSION_MAJOR 3
#define CONTEXT_VERSION_MINOR 3
#define RESOURCES_PATH "../resources/"

#include "utils.hh"
#include "shader.hh"
#include "advanced_math.hh"
#include "callbacks.hh"
#include "quad_fs.hh"

typedef struct {
    bool show_demo_window;
    bool show_component_tree;
} Engine_GUI;

Engine_GUI* init_gui();
void destroy_gui(Engine_GUI* egui);

typedef struct {
    int width, height;
    char* window_title;
    GLFWwindow* window;
    Shader* default_shader_program;
    double mouse_x, mouse_y;
    vec3 camera_position;
    vec3 camera_lookAt;
    int selected_object_id;
    QuadFS* quad;
#ifdef ENABLE_ENGINE_GUI
    Engine_GUI* egui;
    bool enable_gui;
#endif
} Application;

Application* init_application(int width, int height, char* name);
void prepare_to_render(Application* application);
void event_handler(Application* application);
void destroy_application(Application* application);
void draw(Application* application);
void run_application(Application* application);

#endif // APPLICATION_HH