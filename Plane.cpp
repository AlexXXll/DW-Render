#include "Plane.hpp"

Plane::Plane()
    : normal_(Vector3::zero()),
    origin_(Vector3::zero())
{
    
}

Plane::Plane(const Vector3 &normal, const Vector3 &origin)
    : normal_(normal),
    origin_(origin)
{
    
}

Plane Plane::fromTwoVectorsAndPoint(const Vector3 &vec1, const Vector3 &vec2, const Vector3 &point)
{
    // Получение нормали плоскости из векторов
    Vector3 normal = Vector3::cross(vec1, vec2).normalized();
    
    // Создание плоскости из нормали и точки.
    return Plane(normal, point);
}

Plane Plane::fromThreePoints(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
    // Получение два вектора внутри плоскости
    Vector3 vec1 = b - a;
    Vector3 vec2 = c - a;
    
    // Сборка плоскости из двух векторов и точки.
    return fromTwoVectorsAndPoint(vec1, vec2, a);
}
