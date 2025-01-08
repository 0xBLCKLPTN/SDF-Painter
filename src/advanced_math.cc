//
//  advanced_math.cc
//  SDF_ENGINE
//
//  Created by Даниил Ермолаев on 08.01.2025.
//

#include "../include/advanced_math.hh"

vec3 vec3_add(vec3 a, vec3 b) {
#if defined(WIN32)
    return vec3{a.x + b.x, a.y + b.y, a.z + b.z};
#elif defined(__linux__) || defined(__APPLE__)
    return (vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
#endif
}

// Функция для вычитания двух векторов
vec3 vec3_sub(vec3 a, vec3 b) {
#if defined(WIN32)
    return vec3{a.x - b.x, a.y - b.y, a.z - b.z};
#elif defined(__linux__) || defined(__APPLE__)
    return (vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
#endif
}

// Функция для умножения вектора на скаляр
vec3 vec3_mul(vec3 a, float scalar) {
#if defined(WIN32)
    return vec3{a.x * scalar, a.y * scalar, a.z * scalar};
#elif defined(__linux__) || defined(__APPLE__)
    return (vec3) { a.x* scalar, a.y* scalar, a.z* scalar };
#endif
}

// Функция для нормализации вектора
vec3 vec3_normalize(vec3 a) {
    float length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
#if defined(WIN32)
    return vec3{a.x / length, a.y / length, a.z / length};
#elif defined(__linux__) || defined(__APPLE__)
    return (vec3) { a.x / length, a.y / length, a.z / length };
#endif
}

// Функция для вычисления векторного произведения
vec3 vec3_cross(vec3 a, vec3 b) {
#if defined(WIN32)
    return vec3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
#elif defined(__linux__) || defined(__APPLE__)
    return (vec3) {
        a.y* b.z - a.z * b.y,
            a.z* b.x - a.x * b.z,
            a.x* b.y - a.y * b.x
    };
#endif
}
