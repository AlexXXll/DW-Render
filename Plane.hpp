#pragma once

#include "Vector3.hpp"

class Plane
{
public:
    Plane();
    Plane(const Vector3 &normal, const Vector3 &origin);
    
    // Получает нормаль и точку, представляющие плоскость.
    Vector3 normal() const { return normal_; }
    Vector3 origin() const { return origin_; }
    
    // Строит плоскость из двух векторов и точки внутри нее.
    static Plane fromTwoVectorsAndPoint(const Vector3 &vec1, const Vector3 &vec2, const Vector3 &point);
    
    // Строит плоскость из трех точек внутри нее.
    static Plane fromThreePoints(const Vector3 &a, const Vector3 &b, const Vector3 &c);
    
private:
    Vector3 normal_;
    Vector3 origin_;
};
