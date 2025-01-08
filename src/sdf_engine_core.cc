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
    #include "../include/GL/glew.h"
    #include <GLFW/glfw3.h>
#endif

// ================== INCLUDE IMGUI LIBRARIES =============
#if defined(ENABLE_ENGINE_GUI)
    #include "../include/imgui/imgui.h"
    #include "../include/imgui/imgui_impl_glfw.h"
    #include "../include/imgui/imgui_impl_opengl3.h"

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

#include "../include/utils.hh"
#include "../include/shader.hh"
#include "../include/advanced_math.hh"
#include "../include/callbacks.hh"
#include "../include/quad_fs.hh"

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
    bool show_demo_window;
    bool show_component_tree;
    bool enable_gui;
#endif
} Application;

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

#ifdef ENABLE_ENGINE_GUI
  // Setup ImGui
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls.
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#ifdef IMGUI_HASH_DOCK
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking.
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
#if defined(WIN32)
  application->camera_position = vec3{-20.0f, 0.0f, -3.0f}; // Initialize camera position
  application->camera_lookAt = vec3{20.0f, 0.0f, 0.0f};     // Initialize camera direction
#else
  application->camera_position = (vec3){-20.0f, 0.0f, -3.0f}; // Initialize camera position
  application->camera_lookAt = (vec3){20.0f, 0.0f, 0.0f};     // Initialize camera direction
#endif
  application->selected_object_id = -1;

#ifdef ENABLE_ENGINE_GUI
  application->enable_gui = true;
  application->show_demo_window = true; // Ensure demo window is shown
  application->show_component_tree = true;
#else
  application->enable_gui = false;
#endif

  return application;
}

void prepare_to_render(Application* application) {
    // Allocate enough memory to hold the concatenated strings
#if defined(__APPLE__) || defined(__linux__)
    char vertexShaderPath[strlen(RESOURCES_PATH) + strlen("vertex_shader.glsl") + 1];
    char fragmentShaderPath[strlen(RESOURCES_PATH) + strlen("fragment_shader.glsl") + 1];
#else
    char* vertexShaderPath = "../resources/vertex_shader.glsl";
    char* fragmentShaderPath = "../resources/fragment_shader.glsl";
#endif
    // Use sprintf to concatenate the strings
    sprintf(vertexShaderPath, "%s%s", RESOURCES_PATH, "vertex_shader.glsl");
    sprintf(fragmentShaderPath, "%s%s", RESOURCES_PATH, "fragment_shader.glsl");
    application->default_shader_program = create_shader_program(vertexShaderPath, fragmentShaderPath);
    if (!application->default_shader_program || !application->default_shader_program->shader_program) {
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
  glDeleteProgram(application->default_shader_program->shader_program);
  destroy_quadfs(application->quad);
  // Do not free application->window_title as it is a string literal
  free(application);
  glfwTerminate();
}

void draw(Application* application) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(application->window, &fbWidth, &fbHeight);

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
#ifdef ENABLE_ENGINE_GUI
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

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

        if (application->show_demo_window)
            ImGui::ShowDemoWindow(&application->show_demo_window);
        if (application->show_component_tree) {
            ImGui::Begin("Another Window", &application->show_component_tree);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                application->show_component_tree = false;
            ImGui::End();
        }
            

        ImGui::Render();
#endif
        event_handler(application);

        // Check for shader file changes
        time_t current_mod_time = get_file_modification_time("../resources/fragment_shader.glsl");
        if (current_mod_time != last_mod_time) {
            last_mod_time = current_mod_time;
            glDeleteProgram(application->default_shader_program->shader_program);
            prepare_to_render(application);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        draw(application);
#ifdef ENABLE_ENGINE_GUI
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
#endif
        glfwSwapBuffers(application->window);
    }
#ifdef ENABLE_ENGINE_GUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
#endif
    ImGui::DestroyContext();

    destroy_application(application);
    return;
}

int main(int argc, char* argv[])
{
    Application* app = init_application(800, 600, "SDF Engine");
    run_application(app);
    return 0;
}
