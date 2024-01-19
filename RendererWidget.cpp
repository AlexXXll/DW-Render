#include "RendererWidget.hpp"

RendererWidget::RendererWidget()
    : QOpenGLWidget(),
    scene_(),
    renderer_(&scene_),
    targetBitmap_()
{
    // Начинает с понижения разрешения на 1/8 по умолчанию
    resolutionScale_ = RS_EighthRes;
}

void RendererWidget::setResolutionScale(RenderResolution scale)
{
    resolutionScale_ = scale;
    
    // Пересчёт значений, связанных с разрешением
    resizeGL(unscaledResolutionX_, unscaledResolutionY_);
}

void RendererWidget::toggleRenderComponent(RenderComponent component)
{
    renderer_.toggleComponent(component);
    
    // Перезапуск процесса рендеринга, используя новые настройки.
    restartRendering();
}

void RendererWidget::setOverlay(RenderOverlay overlay)
{
    renderer_.setOverlay(overlay);
    
    // Перезапуск процесса рендеринга, используя новую настройку наложения.
    restartRendering();
}

void RendererWidget::restartRendering()
{
    // Остановка любого существующего рендеринга.
    renderer_.stopRender();
    
    // Запуск нового рендера
    renderer_.startRender(&targetBitmap_);
}

void RendererWidget::initializeGL()
{
    // Настройка opengl для рендеринга четырехугольника изображения.
    glDisable(GL_DEPTH_TEST);
    
    // Создание изображения, в которое будет скопировано изображение с трассировкой лучей.
    glGenTextures(1, &targetGPUTexture_);
}

void RendererWidget::resizeGL(int w, int h)
{
    unscaledResolutionX_ = w;
    unscaledResolutionY_ = h;
    
    // Применение понижения разрешения к экрану w и h
    const int scaledWidth = w >> (int)resolutionScale_;
    const int scaledHeight = h >> (int)resolutionScale_;
    
    // Остановка рендера перед изменением размера растрового изображения.
    renderer_.stopRender();
    
    // Выделение памяти для пикселей
    targetBitmap_.resize(scaledWidth, scaledHeight);
    
    // Запустить новый рендер
    restartRendering();
}

void RendererWidget::paintGL()
{
    // Загрузка вычисленного изображения в текстуру OpenGL.
    glBindTexture(GL_TEXTURE_2D, targetGPUTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, targetBitmap_.width(), targetBitmap_.height(), 0, GL_RGB, GL_FLOAT, targetBitmap_.srgbConvert());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // Привязка изображения к GL_TEXTURE_2D
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, targetGPUTexture_);
    
    // Отрисовка на весь экран
    if(contentsRect().width() > unscaledResolutionX_)
    {
        glViewport(0, 0, contentsRect().width(), contentsRect().height());
    }
    else
    {
        glViewport(0, 0, unscaledResolutionX_, unscaledResolutionY_);
    }

    // Отрисовка полноэкранного четырехугольника, содержащего изображение.
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
    
    // Планирует перерисовку немедленно
    update();
}
