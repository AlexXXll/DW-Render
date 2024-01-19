#pragma once

#include "Vector3.hpp"

// Возвращает случайное число от 0 до 1.
float random_float();

// Возвращает случайное число между минимумом и максимумом.
float random_float(float min, float max);

// Возвращает случайный вектор в верхней полусфере.
// Равная вероятность для каждого направления.
Vector3 random_hemisphere_ray(const Vector3 &normal);

// Возвращает случайный вектор в верхней полусфере.
// Используется выборка по важности - вероятность выбора.
// направление d — это точка d, нормальная.
Vector3 random_hemisphere_ray_is(const Vector3 &normal);
