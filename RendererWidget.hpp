#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QImage>
#include <cstdint>
#include <math.h>

#include "Bitmap.hpp"
#include "Color.hpp"
#include "RendererSettings.hpp"
#include "RendererStats.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"

class RendererWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    RendererWidget();
    
    Scene* scene() { return &scene_; }
    
    // Информация о рендеринге
    const RendererStats* stats() const { return renderer_.stats(); }
    
    // Текущие настройки рендеринга
    int resolutionX() const { return targetBitmap_.width(); }
    int resolutionY() const { return targetBitmap_.height(); }
    RenderResolution resolutionScale () const { return resolutionScale_; }
    
    // Камера основной сцены
    Camera* camera() { return scene_.mainCamera(); }
    
    // Изменяет настройки рендеринга.
    void setResolutionScale(RenderResolution scale);
    void toggleRenderComponent(RenderComponent component);
    void setOverlay(RenderOverlay overlay);
    
    // (Повторно) запускает процесс рендеринга.
    // Это следует вызывать всякий раз, когда меняется сцена/камера.
    void restartRendering();
    
private:
    // Сцена, которая рендерится.
    // Содержит список цветных triangles.
    Scene scene_;
    
    // Используемый рендерер
    Renderer renderer_;
    
    // Изображение, которое отображается в
    Bitmap targetBitmap_;
    GLuint targetGPUTexture_;
    
    // Настройки рендеринга
    int unscaledResolutionX_;
    int unscaledResolutionY_;
    RenderResolution resolutionScale_;
    
    // Методы переопределения QGLWidget
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};
