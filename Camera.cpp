#include "Camera.hpp"

#include <assert.h>
#include <math.h>

Camera::Camera()
    : nearPlaneDistance_(0.1),
    fov_(60.0)
{
    
}

void Camera::setNearPlane(float nearPlane)
{
    nearPlaneDistance_ = nearPlane;
}

void Camera::setFov(float fov)
{
    fov_ = fov;
}

Vector3 Camera::getViewRayDir(float x, float y, float aspect) const
{
    // Рассчёт ширины и высоты плоскости обзора на расстоянии ближней плоскости.
    float halfFov = (fov_ / 2.0) * (M_PI / 180.0);
    float planeWidth = nearPlaneDistance_ * tan(halfFov) * 2.0f;
    float planeHeight = planeWidth * aspect;
    
    // Определение центра плоскости обзора на расстоянии ближней плоскости.
    Vector3 origin = position();
    Vector3 planeCentre = origin + (forward() * nearPlaneDistance_);
    
    // Смещение к правильному местоположению внутри плоскости
    Vector3 xOffset = right() * (x - 0.5f) * planeWidth;
    Vector3 yOffset = up() * (y - 0.5f) * planeHeight;
    
    // Смещение центра плоскости, чтобы получить точку в плоскости на луче просмотра.
    Vector3 planeRayIntersection = planeCentre + xOffset + yOffset;
    
    // Возвращение положения в близкое к плоскости направление.
    return (planeRayIntersection - origin).normalized();
}
