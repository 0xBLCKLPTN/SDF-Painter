#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>

typedef struct {
    float x, y, z;
} vec3;


// Функция для сложения двух векторов
vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

// Функция для вычитания двух векторов
vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

// Функция для умножения вектора на скаляр
vec3 vec3_mul(vec3 a, float scalar) {
    return (vec3){a.x * scalar, a.y * scalar, a.z * scalar};
}

// Функция для нормализации вектора
vec3 vec3_normalize(vec3 a) {
    float length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    return (vec3){a.x / length, a.y / length, a.z / length};
}

// Функция для вычисления векторного произведения
vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
#endif // VEC3_H_
