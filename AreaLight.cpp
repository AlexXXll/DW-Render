#include "AreaLight.hpp"

AreaLight::AreaLight(const Vector3 &min, const Vector3 &max, const Color &emission)
    : min_(min), size_(max - min), emission_(emission)
{
    // Получение двух векторов внутри области освещения.
    const Vector3 v1 = (max - min).normalized();
    const Vector3 v2 = (Vector3(min.x, min.y, max.z) - min).normalized();

    // Использование векторов, чтобы получить нормаль
    normal_ = Vector3::cross(v1, v2).normalized();
}

Vector3 AreaLight::getPoint() const
{
    // Вычисление случайных смещений во всех трех направлениях
    const float xOffset = (rand() / (float)RAND_MAX) * size_.x;
    const float yOffset = (rand() / (float)RAND_MAX) * size_.y;
    const float zOffset = (rand() / (float)RAND_MAX) * size_.z;
    
    // Возвращение точки с указанным смещением
    return min_ + Vector3(xOffset, yOffset, zOffset);
}
