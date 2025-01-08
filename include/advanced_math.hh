//
//  advanced_math.cc
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#ifndef ADVANCED_MATH_HH_
#define ADVANCED_MATH_HH_

#include <math.h>

typedef struct {
    float x, y, z;
} vec3;

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float scalar);
vec3 vec3_normalize(vec3 a);
vec3 vec3_cross(vec3 a, vec3 b);

#endif // ADVACNED_MATH_HH_
