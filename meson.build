project('SDF Engine', 'c', 'cpp', version: '0.1.0', default_options : ['cpp_std=c++11'])

cc = meson.get_compiler('c')

<<<<<<< HEAD
glfw_dep = dependency('glfw3', method: 'pkg-config')
glew_dep = dependency('glew', method: 'pkg-config')
gl_dep = dependency('gl', method: 'pkg-config')
opencl_dep = dependency('OpenCL', method: 'pkg-config')
=======

glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
gl_dep = dependency('gl')
opencl_dep = dependency('OpenCL')
>>>>>>> f087f28a7f7228dcd445f7f91c9816ec3001560b
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

<<<<<<< HEAD
executable('sdfe', sources, dependencies: [math_dep, glfw_dep, glew_dep, gl_dep, opencl_dep], include_directories: include_dirs)
=======
executable('sdfe', sources, dependencies: [math_dep, glfw_dep, glew_dep, gl_dep, opencl_dep])
>>>>>>> f087f28a7f7228dcd445f7f91c9816ec3001560b
