#pragma once

#include <iostream>

using namespace std;

#include "Vector3.hpp"

struct Quaternion
{
    Vector3 v;
    float w;

    Quaternion();
    Quaternion(const Vector3 &v, float w);
    Quaternion(float x, float y, float z, float w);
    
    static Quaternion identity();

    float sqrNorm() const;
    float norm() const;
    Quaternion conjugate() const;
    Quaternion inverse() const;

    // Создает поворот на указанный угол (градусы) вокруг заданной оси.
    static Quaternion rotation(float angle, const Vector3 &axis);

    // Создает поворот на z градусов вокруг оси z, затем на y градусов вокруг оси y,
    // а затем на x градусов вокруг оси x.
    static Quaternion euler(float x, float y, float z);
    static Quaternion euler(Vector3 &euler);
};

Vector3 operator * (const Quaternion &q, const Vector3 &vec);

Quaternion operator * (const Quaternion &q, float scalar);
Quaternion operator * (float scalar, const Quaternion &q);

Quaternion operator * (const Quaternion &a, const Quaternion &b);
Quaternion operator + (const Quaternion &a, const Quaternion &b);

ostream& operator << (ostream &os, Quaternion &quat);
istream& operator >> (istream &is, Quaternion &quat);
