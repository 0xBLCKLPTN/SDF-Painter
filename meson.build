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
  'include/cimgui/cimgui.cpp',
  'include/cimgui/imgui/imgui.cpp',
  'include/cimgui/imgui/imgui_draw.cpp',
  'include/cimgui/imgui/imgui_tables.cpp',
  'include/cimgui/imgui/imgui_widgets.cpp',
  'include/cimgui/imgui/backends/imgui_impl_glfw.cpp',
  'include/cimgui/imgui/backends/imgui_impl_opengl3.cpp',
  'include/cimgui/imgui/imgui_demo.cpp',
]

# Include directories
include_dirs = [
  'src',
  'include',
  'include/cimgui/imgui/backends',
  'include/cimgui',
  'include/cimgui/imgui',
  'include/cimgui/generator/output'
]

# Create the executable
executable('sdfe', sources,
  dependencies: [glfw_dep, glew_dep, gl_dep, math_dep],
  include_directories: include_dirs
)
