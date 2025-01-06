project('SDF Engine', 'c', 'cpp', version: '0.1.0', default_options : ['cpp_std=c++11'])

# Get the C compiler
cc = meson.get_compiler('c')

# Define dependencies
glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
gl_dep = dependency('gl')
math_dep = cc.find_library('m', required: true)

# Source files
sources = [
  'src/main.cc',
  'src/quad_fs.cc',
  'src/callbacks.cc',
  'include/imgui/imgui.cpp',
  'include/imgui/imgui_draw.cpp',
  'include/imgui/imgui_tables.cpp',
  'include/imgui/imgui_widgets.cpp',
  'include/imgui/imgui_impl_glfw.cpp',
  'include/imgui/imgui_impl_opengl3.cpp',
  'include/imgui/imgui_demo.cpp',
]

# Include directories
include_dirs = [
  'src',
  'include',
  'include/imgui',
]

# Create the executable
executable('sdfe', sources,
  dependencies: [glfw_dep, glew_dep, gl_dep, math_dep],
  include_directories: include_dirs
)
