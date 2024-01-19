#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QImage>
#include <cstdint>
#include <math.h>
#include <vector>
#include <thread>

#include "Bitmap.hpp"
#include "Color.hpp"
#include "RendererSettings.hpp"
#include "RendererStats.hpp"
#include "Scene.hpp"
#include "SurfaceHit.hpp"

class Renderer
{
public:
    Renderer(const Scene* scene);
    
    // Информация о рендеринге
    const RendererStats* stats() const { return &stats_; }
    
    bool running() const { return rendering_; }
    bool componentEnabled(RenderComponent component) const;
    
    // Изменяет настройки рендеринга теней
    void setOverlay(RenderOverlay overlay);
    void toggleComponent(RenderComponent component);
    
    // Запускает и останавливает рендеринг.
    // Рендеринг происходит в фоновом потоке.
    void startRender(Bitmap* bitmap);
    void stopRender();
    
private:
    const Scene* scene_;
    
    // Журналы статистики рендеринга для отображения в пользовательском интерфейсе.
    RendererStats stats_;
    
    // Какие компоненты рендеринга в настоящее время включены.
    RenderComponentList renderComponents_;
    
    // Какое наложение рендеринга используется в данный момент.
    RenderOverlay renderOverlay_;
    
    // True, если выполняется проход рендеринга.
    bool rendering_;
    
    // Рабочие потоки
    std::vector<std::thread> renderThreads_;
    
    // Методы переопределения QGLWidget
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    
    void renderWorker(Bitmap* bitmap);
    
    // Вычисляет яркость для трассируемого луча..
    Color pathTrace(const Ray &viewRay, int bounce = 0) const;
    
    // Вычисляет яркость в заданной точке по источникам света области сцены.
    Color sampleDirectAreaLights(const Ray &viewRay, const SurfaceHit &hit) const;
    
    // Вычисляет импульсное излучение в заданной точке
    Color sampleImpulseLight(const Ray &viewRay, const SurfaceHit &hit, int bounce) const;
    
    // Вычисляет косвенное излучение в заданной точке.
    Color sampleIndirectLight(const SurfaceHit &hit, int bounce) const;
    
    // Находит объект, на который попадает луч, если таковой имеется.
    SurfaceHit testRay(const Ray &ray) const;
    
    // Проверяет наличие прямой видимости между двумя точками.
    // Возвращает true, если нет препятствий.
    bool checkVisibility(const Vector3 &a, const Vector3 &b) const;
    
    // Вычисляет излучение блинфонга при попадании на поверхность.
    Color blinnPhong(const SurfaceHit &surface, const Vector3 &toEye, const Vector3 &toLight) const;
};
