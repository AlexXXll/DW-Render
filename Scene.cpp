#include "Scene.hpp"

#include <cstdio>
#include <assert.h>

Scene::Scene()
    : camera_(),
    ambientLight_(Color(0.8245026f, 0.8790448f, 0.8970588f)),
    lights_(),
    areaLights_(),
    triangles_(),
    spheres_()
{
    // Расположение камеры в выбранном вручную месте.
    camera_.setPosition(Vector3(0.0f, 5.0f, -15.0f));
    camera_.setRotation(Quaternion::identity());
    
    // Создание света и геометрии
    createWalls();
    createFloor();
    createCeiling();
    createAreaLight();
    
    // Создание нескольких сфер в центре сцены.
    addSphere(Sphere(Color::white(), Vector3(-2.2f, 2.0f, 0.0f), 2.0f));
    addSphere(Sphere(Color::white(), Vector3(2.2f, 2.0f, 0.0f), 2.0f));
    addSphere(Sphere(Color::blue(), Vector3(-1.5f, 1.2f, -3.0f), 1.2f, 0.1f));
    addSphere(Sphere(Color::blue(), Vector3(1.5f, 1.2f, -3.0f), 1.2f, 0.1f));
    addSphere(Sphere(Color::blue(), Vector3(0.0f, 1.2f, 3.0f), 1.2f, 0.1f));
}

void Scene::createWalls()
{
    // Левая стена (красная)
    const Vector3 left1 = Vector3(-5.0f, 0.0f, 5.0f);
    const Vector3 left2 = Vector3(-5.0f, 0.0f, -5.0f);
    const Color leftColor = Color::red();
    addWall(left1, left2, leftColor);
    
    // Правая стена (зеленая)
    const Vector3 right1 = Vector3(5.0f, 0.0f, -5.0f);
    const Vector3 right2 = Vector3(5.0f, 0.0f, 5.0f);
    const Color rightColor(0.7f, 1.0f, 0.85f);
    addWall(right1, right2, rightColor);
    
    // Передняя стенка (серая)
    const Vector3 front1 = Vector3(-5.0f, 0.0f, -5.0f);
    const Vector3 front2 = Vector3(5.0f, 0.0f, -5.0f);
    const Color frontColor = Color::grey();
    addWall(front1, front2, frontColor);
    
    // Задняя стенка (серая)
    const Vector3 back1 = Vector3(5.0f, 0.0f, 5.0f);
    const Vector3 back2 = Vector3(-5.0f, 0.0f, 5.0f);
    const Color backColor = Color::grey();
    addWall(back1, back2, backColor);
}

void Scene::createFloor()
{
    // Пол серый
    const Color color = Color::grey();
    
    // Пол представляет собой квадрат с 4 вершинами.
    const Vector3 p1 = Vector3(-5.0f, 0.0f, 5.0f);
    const Vector3 p2 = Vector3(5.0f, 0.0f, 5.0f);
    const Vector3 p3 = Vector3(5.0f, 0.0f, -5.0f);
    const Vector3 p4 = Vector3(-5.0f, 0.0f, -5.0f);
    
    // Создание пола из 2 треугольников.
    addTriangle(Triangle(p1, p2, p3, color), false);
    addTriangle(Triangle(p1, p3, p4, color), false);
}

void Scene::createCeiling()
{
    // Потолок серый
    const Color color = Color::grey();
    
    // Потолок представляет собой четырехугольник с 4 вершинами.
    const Vector3 p1 = Vector3(-5.0f, 10.0f, -5.0f);
    const Vector3 p2 = Vector3(5.0f, 10.0f, -5.0f);
    const Vector3 p3 = Vector3(5.0f, 10.0f, 5.0f);
    const Vector3 p4 = Vector3(-5.0f, 10.0f, 5.0f);
    
    // Создание потолка из 2 треугольников
    addTriangle(Triangle(p1, p2, p3, color), false);
    addTriangle(Triangle(p1, p3, p4, color), false);
}

void Scene::createAreaLight()
{
    // Свет черный, с белым излучением
    const Color color = Color::black();
    const Color emission(2000.0f, 2000.0f, 2000.0f);
    
    // Зональный свет представляет собой квадрат
    // Смещение немного ниже потолка.
    const float eps = 0.000001f;
    const Vector3 p1 = Vector3(-1.0f, 10.0f - eps, -1.0f);
    const Vector3 p2 = Vector3(1.0f, 10.0f - eps, -1.0f);
    const Vector3 p3 = Vector3(1.0f, 10.0f - eps, 1.0f);
    const Vector3 p4 = Vector3(-1.0f, 10.0f - eps, 1.0f);
    
    // Создание области света из 2 треугольников.
    addTriangle(Triangle(p1, p2, p3, color, emission), false);
    addTriangle(Triangle(p1, p3, p4, color, emission), false);
    
    // Создание источника света, соответствующий треугольникам.
    areaLights_.push_back(AreaLight(p1, p3, emission));
}

void Scene::addWall(const Vector3 &b1, const Vector3 &b2, const Color &color)
{
    // Стена представляет собой четырехугольник с 4 вершинами.
    const Vector3 p1 = Vector3(b1.x, 0.0f, b1.z);
    const Vector3 p2 = Vector3(b2.x, 0.0f, b2.z);
    const Vector3 p3 = Vector3(b2.x, 10.0f, b2.z);
    const Vector3 p4 = Vector3(b1.x, 10.0f, b1.z);
    
    // Создание стены из двух треугольников.
    addTriangle(Triangle(p1, p2, p3, color), false);
    addTriangle(Triangle(p1, p3, p4, color), false);
}

void Scene::addTriangle(const Triangle &triangle, bool doubleSided)
{
    // Добавление треугольника на сцену.
    triangles_.push_back(triangle);

    if(doubleSided)
    {
        // Смена местами обмотки треугольника и добавление ее ещё раз,
        // чтобы сформировать заднюю часть треугольника.
        Triangle reversed(triangle.vertices[2], triangle.vertices[1], triangle.vertices[0], triangle.color);
        
        // Добавление перевернутого треугольника в список треугольников.
        triangles_.push_back(reversed);
    }
}

void Scene::addSphere(const Sphere &sphere)
{
    spheres_.push_back(sphere);
}

void Scene::addLight(const Light &light)
{
    // Добавление света в сцену.
    lights_.push_back(light);
}
