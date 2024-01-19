#pragma once

#include "Object.hpp"
#include "Ray.hpp"

class Camera : public Object
{
public:
    Camera();
    
    // Конфигурация камеры
    float nearPlane() const { return nearPlaneDistance_; }
    float fov() const { return fov_; }
    
    // установка конфигурации камеры
    void setNearPlane(float nearPlane);
    void setFov(float fov);
    
    // Получение направления луча от камеры через плоскость обзора
    // для нормализованной позиции [0-1] x/y на плоскости обзора
    Vector3 getViewRayDir(float x, float y, float aspect) const;
    
private:
    // Направления лучей для 4 углов усеченной пирамиды обзора
    Vector3 frustumCorners_;
    
    // Расстояние до ближайших плоскостей отсечения
    float nearPlaneDistance_;
    
    // Вертикальное поле зрения
    float fov_;
};
