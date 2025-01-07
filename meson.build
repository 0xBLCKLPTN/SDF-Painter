project('SDF Engine', 'c', 'cpp', version: '0.1.0', default_options : ['cpp_std=c++11'])

# Get the C compiler
cc = meson.get_compiler('c')


glfw3_subproj = subproject('glfw')
glfw3_dep = glfw3_subproj.get_variable('glfw_dep')
glew_dep = dependency('glew')
gl_dep = dependency('gl')
math_dep = cc.find_library('m', required: true)
lib_rt = cc.find_library('rt', required : get_option('build-lsfd'))
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

# Add GLFW as a subproject
glfw_subproject = subproject('glfw')
glfw_dep = glfw_subproject.get_variable('glfw3_dep')

# Create the executable
executable('sdfe', sources,
  dependencies: [glfw3_dep, glew_dep, gl_dep, math_dep, lib_rt],
  include_directories: include_dirs
)
