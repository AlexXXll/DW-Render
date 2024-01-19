#pragma once

#include "Vector3.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

class Ray
{
public:
    Ray(const Vector3 &origin, const Vector3 &direction);
    
    Vector3 origin() const { return origin_; }
    Vector3 direction() const { return direction_; }
    
    // Вычисляет точку пересечения плоскости и луча, если таковая имеется.
    bool intersectionWithPlane(const Plane &plane, Vector3 &intersection, float &distance) const;
    
    // Вычисляет пересечение луча и треугольника.
    // Возвращает расстояние от начала луча до пересечения.
    // Также выводит барицентрические координаты треугольника.
    bool intersectionWithTriangle(const Triangle &triangle, float &alpha, float &beta, float &gamma, float &distance) const;
    
    // Вычисляет пересечение луча и сферы.
    // Выводит расстояние от пересечения.
    bool intersectionWithSphere(const Sphere &sphere, float &distance) const;

private:
    Vector3 origin_;
    Vector3 direction_;
};
 
