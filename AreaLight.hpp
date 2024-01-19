#pragma once

#include "Vector3.hpp"
#include "Color.hpp"

// Квадратный точечный светильник, выровненный по оси
class AreaLight
{
public:
    AreaLight(const Vector3 &min, const Vector3 &max, const Color &emission);
    
    // Излучение света
    Color emission() const { return emission_; }
    
    // Геометрическая нормаль площадного освещения
    Vector3 normal() const { return normal_; }

    // Получение случайной точки на поверхности источника света
    Vector3 getPoint() const;
    
private:
    Vector3 min_;
    Vector3 size_;
    Vector3 normal_;
    Color emission_;
};
