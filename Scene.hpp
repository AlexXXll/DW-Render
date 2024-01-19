#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

#include "AreaLight.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Object.hpp"

class Scene
{
public:
    Scene();
    
    // Зрительская viewer-а
    const Camera* mainCamera() const { return &camera_; }
    Camera* mainCamera() { return &camera_; }
    
    // Настройки света
    const Color& ambientLight() const { return ambientLight_; }
    const Light& mainLight() const { return lights_[0]; }
    const Light& bigRedLight() const { return lights_[1]; }
    
    // Компоненты сцены
    const std::vector<Light>& lights() const { return lights_; }
    const std::vector<AreaLight>& areaLights() const { return areaLights_; }
    const std::vector<Triangle>& triangles() const { return triangles_; }
    const std::vector<Sphere>& spheres() const { return spheres_; }
    
    // Создает 3 стенки коробки Корнелла.
    void createWalls();
    
    // Создает пол коробки Корнелла
    void createFloor();
    
    // Создает потолок Корнеллской коробки
    void createCeiling();
    
    // Создает свет области в верхней части коробки Корнелла.
    void createAreaLight();
    
    // Добавляет стену с указанными базовыми позициями и цветом.
    void addWall(const Vector3 &b1, const Vector3 &b2, const Color &color);
    
    // Добавляет указанный треугольник на сцену.
    void addTriangle(const Triangle &triangle, bool doubleSided = true);
    
    // Добавляет указанную сферу на сцену.
    void addSphere(const Sphere &sphere);

    // Добавляет указанный источник света в сцену.
    void addLight(const Light &light);
    
private:
    // Камера сцены.
    Camera camera_;
    
    // Количество окружающего света, применяемого ко всем объектам.
    Color ambientLight_;
    
    // Компоненты сцены
    std::vector<Light> lights_;
    std::vector<AreaLight> areaLights_;
    std::vector<Triangle> triangles_;
    std::vector<Sphere> spheres_;
};
