//
//  utils.hh
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#ifndef UTILS_HH
#define UTILS_HH

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(__APPLE__) || defined(__linux__)
    #include <unistd.h>
#endif

char* read_file(const char* file_path);
time_t get_file_modification_time(const char* file_path);


#endif // UTILS_HH
