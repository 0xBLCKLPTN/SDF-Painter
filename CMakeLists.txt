cmake_minimum_required(VERSION 3.15)
project(SDF_ENGINE VERSION 2.0.0 LANGUAGES C CXX)

# CMAKE CONFIGURATION
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/application.cc
    src/sdf_engine_core.cc
    src/advanced_math.cc
    src/shader.cc
    src/utils.cc
    src/callbacks.cc
    src/quad_fs.cc
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_demo.cpp
)

if (UNIX OR APPLE)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
else()
    # Detect MSVC version and set GLFW library path accordingly
    if (CMAKE_GENERATOR MATCHES "Visual Studio 17 2022")
        set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glfw/lib-vc2022/glfw3.lib)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 16 2019")
        set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glfw/lib-vc2019/glfw3.lib)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 15 2017")
        set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glfw/lib-vc2017/glfw3.lib)
    else()
        message(FATAL_ERROR "Unsupported MSVC version: ${CMAKE_GENERATOR}")
    endif()

    set(GLFW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/win_libs/glfw/include)
    set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/win_libs/glew/include)
    set(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glew/lib/Release/x64/glew32.lib)
endif(UNIX OR APPLE)

# All include files such as *.h
if (UNIX OR APPLE)
    set(INCLUDE_DIRS
      src
      include
      include/imgui
    )
else()
    set(INCLUDE_DIRS
        src
        include
        include/imgui
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        include/GL/eglew.h
        include/GL/glew.h
        include/GL/glxew.h
        include/GL/wglew.h
    )
endif(UNIX OR APPLE)

# Create executable
add_executable(sdfe ${SOURCES})


# Include Directories
target_include_directories(sdfe PRIVATE ${INCLUDE_DIRS})

# Link Libraries
if (UNIX OR APPLE)
    target_link_libraries(sdfe PRIVATE GLEW::GLEW glfw)
else()
    target_link_libraries(sdfe PRIVATE ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
endif(UNIX OR APPLE)

if (WIN32)
    target_link_libraries(sdfe PRIVATE OpenGL32)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
endif(WIN32)
