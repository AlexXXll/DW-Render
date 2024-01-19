#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

class Object
{
public:
    Object();
    
    // Настройки трансформации
    Vector3 position() const { return position_; }
    Quaternion rotation() const { return rotation_; }
    Vector3 scale() const { return scale_; }
    
    // Матрицы преобразования
    Matrix4x4 worldToLocal() const { return worldToLocal_; }
    Matrix4x4 localToWorld() const { return localToWorld_; }
    
    // Ось объекта в общем пространстве
    Vector3 forward() const;
    Vector3 up() const;
    Vector3 right() const;
    
    // Преобразование вектора направления из локального в общее пространство
    Vector3 localToWorldVector(const Vector3 &v) const;
    
    // Непосредственно устанавливает преобразование
    void setPosition(const Vector3 &pos);
    void setRotation(const Quaternion &rot);
    void setScale(const Vector3 &scale);
    
    // Перемещает объект по заданному вектору общего пространства.
    void translate(const Vector3 &translation);
    
    // Вращает объект вокруг заданного вектора мирового пространства.
    // Угол измеряется в градусах.
    void rotate(float angle, const Vector3 &axis);

private:
    
    // Настройки трансформации
    Vector3 position_;
    Quaternion rotation_;
    Vector3 scale_;
    
    // Кэшированние матрицы преобразования
    Matrix4x4 worldToLocal_;
    Matrix4x4 localToWorld_;
    
    void recreateTransformation();
};
