#pragma once

#include <iostream>
#include <cstdint>

#include "Vector3.hpp"

using namespace std;

struct Color
{
    float r;
    float g;
    float b;
    
    Color();
    Color(float r, float g, float b);
    Color(const Vector3 &v);
    
    static Color black();
    static Color white();
    static Color grey();
    static Color red();
    static Color green();
    static Color blue();
    static Color skyBlue();
    
    Color& operator+=(const Color &other);
    
    Color operator/ (float scalar) const;
    Color operator+ (Color c) const;
    Color operator- (Color c) const;
    
    // Преобразует одно скалярное значение из srgb в линейное
    static float gammaToLinear(float srgb);
    static float linearToGamma(float linear);
    
    // Преобразует цвет из гаммы в линейное пространство.
    // Альфа (если указана) не изменяется и 1, если не указана.
    static Color gammaToLinear(float r, float g, float b);
    static Color gammaToLinear(const Color& color);
    
    // Преобразует цвет из линейного в гамма-пространство.
    // Альфа не изменяется, если указана, и 1, если не указана.
    static Color linearToGamma(float r, float g, float b);
    static Color linearToGamma(const Color& color);
    
    // Выполняет линейную интерполяцию от a до b
    static Color lerp(const Color &a, const Color &b, float t);
};

Color operator * (const Color &color, float scalar);
Color operator * (float scalar, const Color &color);
Color operator * (const Color &a, const Color &b);
Color operator + (const Color &color, float scalar);

ostream& operator<< (ostream &os, const Color &color);
istream& operator>> (istream &is, Color &color);
