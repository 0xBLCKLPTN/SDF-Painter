cmake_minimum_required(VERSION 3.10)
project(SDF_Engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMAKE_TOOLCHAIN_FILE to the path of the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Find packages
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Source files
set(SOURCES
  src/main.cc
  src/quad_fs.cc
  src/callbacks.cc
  include/imgui/imgui.cpp
  include/imgui/imgui_draw.cpp
  include/imgui/imgui_tables.cpp
  include/imgui/imgui_widgets.cpp
  include/imgui/imgui_impl_glfw.cpp
  include/imgui/imgui_impl_opengl3.cpp
  include/imgui/imgui_demo.cpp
)

# Include directories
set(INCLUDE_DIRS
  src
  include
  include/imgui
)

# Create the executable
add_executable(sdfe ${SOURCES})

# Include directories
target_include_directories(sdfe PRIVATE ${INCLUDE_DIRS})

# Link libraries
target_link_libraries(sdfe PRIVATE GLEW::GLEW glfw)
