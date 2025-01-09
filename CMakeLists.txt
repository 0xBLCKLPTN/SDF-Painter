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

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH "x64")
else()
  set(ARCH "x32")
endif()

if (UNIX OR APPLE)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
else()
    # Cross-compilation for Windows
    if (ARCH EQUAL "x64")
        set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glfw_x64/lib-vc2022/glfw3.lib)
        set(GLFW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/x64/GLFW)
        set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/win_libs/glew/include)
        set(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glew/lib/Release/x64/glew32.lib)
    else()
        set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glfw_x32/lib-vc2022/glfw3.lib)
        set(GLFW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/x32/GLFW)
        set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/win_libs/glew/include)
        set(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/win_libs/glew/lib/Release/Win32/glew32.lib)
    endif()
endif()

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
        ${GLEW_INCLUDE_DIRS}
    )
    # Add GLFW include directory to the system include path
    include_directories(SYSTEM ${GLFW_INCLUDE_DIRS})
endif()

if (NOT APPLE)
    add_executable(sdfe ${SOURCES})

    # Include Directories
    target_include_directories(sdfe PRIVATE ${INCLUDE_DIRS})

    # Link Libraries
    if (UNIX OR APPLE)
        target_link_libraries(sdfe PRIVATE GLEW::GLEW glfw)
    else()
        target_link_libraries(sdfe PRIVATE ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
    endif()
endif()

if (WIN32)
    target_link_libraries(sdfe PRIVATE OpenGL32)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
endif()

if (WIN32)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_NAME "MSVC")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME "GCC")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_NAME "Clang")
  else()
    set(COMPILER_NAME "Unknown")
  endif()

  add_custom_command(TARGET sdfe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sdfe> ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}/bin
    COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_DLL_PATH} ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}/bin
    COMMAND ${CMAKE_COMMAND} -E copy ${GLEWS_DLL_PATH} ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}/bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/resources ${CMAKE_BINARY_DIR}/SDF-Engine_WIN-x32-64_${COMPILER_NAME}
  )
endif()

if (APPLE)
    set(CUR_TARGET sdfe)
    add_executable(${CUR_TARGET} MACOSX_BUNDLE ${SOURCES})
    target_include_directories(sdfe PRIVATE ${INCLUDE_DIRS})
    target_link_libraries(sdfe PRIVATE GLEW::GLEW glfw)
    set_target_properties(${CUR_TARGET} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${CUR_TARGET}
        MACOSX_BUNDLE_BUNDLE_NAME ${CUR_TARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
    )

    # Copy resources to the macOS application bundle
    add_custom_command(TARGET ${CUR_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${CUR_TARGET}.app/Contents/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/${CUR_TARGET}.app/Contents/Resources
    )

    # Copy the icon file to the macOS application bundle
    add_custom_command(TARGET ${CUR_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/icon.icns ${CMAKE_BINARY_DIR}/${CUR_TARGET}.app/Contents/Resources/icon.icns
    )
endif()
