project('SDF Engine', 'c', 'cpp', version: '0.1.0', default_options : ['cpp_std=c++11'])

cc = meson.get_compiler('c')


glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
gl_dep = dependency('gl')
opencl_dep = dependency('OpenCL')
math_dep = cc.find_library('m', required: true)

sources = [
  'src/main.c',
  'src/quad_fs.c'
]

include_dirs = [
  './src',
  './include',
  './include/imgui',
]

executable('sdfe', sources, dependencies: [math_dep, glfw_dep, glew_dep, gl_dep, opencl_dep])
