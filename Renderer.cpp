#include "Renderer.hpp"

#include "Random.hpp"

Renderer::Renderer(const Scene* scene)
    : scene_(scene),
    stats_(),
    renderComponents_(~0),
    renderOverlay_(RO_None),
    rendering_(false),
    renderThreads_()
{
    
}

bool Renderer::componentEnabled(RenderComponent component) const
{
    return (renderComponents_ & component) != 0;
}

void Renderer::setOverlay(RenderOverlay overlay)
{
    renderOverlay_ = overlay;
}

void Renderer::toggleComponent(RenderComponent component)
{
    if(componentEnabled(component))
    {
        renderComponents_ &= (~component);
    }
    else
    {
        renderComponents_ |= component;
    }
}

void Renderer::startRender(Bitmap* bitmap)
{
    // Остановка любого существующего рендера
    stopRender();
    
    // Очистка растровоого изображения перед началом нового рендеринга.
    bitmap->clear();
    
    // Запуск нового рендера
    rendering_ = true;
    stats_.renderStarted(bitmap->width(), bitmap->height());
    
    // Запуск нескольких рабочих потоков для отслеживания пути
    const int threadCount = 4;
    for(int thread = 0; thread < threadCount; ++thread)
    {
        renderThreads_.push_back(std::thread(&Renderer::renderWorker, this, bitmap));
    }
}

void Renderer::stopRender()
{
    // Отметить рендерер как остановленный.
    rendering_ = false;
    
    // Ожидание завершения потоков рендеринга.
    for(unsigned int i = 0; i < renderThreads_.size(); ++i)
    {
        renderThreads_[i].join();
    }
    
    // Удаление всех завершённых рабочих потоков.
    renderThreads_.clear();
}

void Renderer::renderWorker(Bitmap *bitmap)
{
    const float pixelWidth = 1.0f / (float)bitmap->width();
    const float pixelHeight = 1.0f / (float)bitmap->height();
    const float aspect = pixelWidth / pixelHeight;
    
    // Постоянное создание большего колличества образцов пикселей, пока не будет сообщено остановиться.
    while(rendering_)
    {
        // Выбор рандомного пикселя
        const int x = (rand() % bitmap->width());
        const int y = (rand() % bitmap->height());
        
        // Смещение координаты в случайную точку внутри пикселя.
        const float normalizedX = (x / (float)bitmap->width()) + random_float(0.0f, pixelWidth);
        const float normalizedY = (y / (float)bitmap->height()) + random_float(0.0f, pixelHeight);
        
        // Получение viewRay.
        const Vector3 viewRayOrigin = scene_->mainCamera()->position();
        const Vector3 viewRayDir = scene_->mainCamera()->getViewRayDir(normalizedX, normalizedY, aspect);
        const Ray viewRay(viewRayOrigin, viewRayDir);
        
        // Трассировка пути по одному образцу для пикселя
        bitmap->addSample(x, y, pathTrace(viewRay));
        
        // Сохранение количества образца
        stats_.addSample();
    }
}

Color Renderer::pathTrace(const Ray &viewRay, int bounce) const
{
    // Обработка наложения направлений лучей
    if(renderOverlay_ == RO_PixelRays)
    {
        return viewRay.direction() * 0.5f + 0.5f;
    }
    
    // Находит и обработывает ближайшую поверхность, которую пересекает луч.
    const SurfaceHit hit = testRay(viewRay);
    if(hit.distance == INFINITY)
    {
        // Делает цвет неба от светло-голубого до темно-синего.
        return Color::lerp(Color::white(), Color::skyBlue(), viewRay.direction().y * 0.5f + 0.5f);
    }
    
    // Обработка наложений информации о попаданиях
    if(renderOverlay_ == RO_Distance) return Color::white() * (1.0f - hit.distance * 0.01f);
    if(renderOverlay_ == RO_Position) return hit.position * 0.1f + 0.5f;
    if(renderOverlay_ == RO_Color) return hit.color;
    if(renderOverlay_ == RO_Normals) return hit.normal * 0.5f + 0.5f;
    if(renderOverlay_ == RO_Emission) return hit.emission * 0.0002f;
    
    // Сохраняет сумму сияния вдоль пути
    Color radiance = Color::black();

    if(bounce == 0)
    {
        radiance += hit.emission * 0.0005f;
    }
    
    if(componentEnabled(RC_Direct)) radiance += sampleDirectAreaLights(viewRay, hit);
    if(componentEnabled(RC_Impulse) && bounce == 0) radiance += sampleImpulseLight(viewRay, hit, bounce);
    if(componentEnabled(RC_Indirect)) radiance += sampleIndirectLight(hit, bounce);
    return radiance;
}

Color Renderer::sampleDirectAreaLights(const Ray &viewRay, const SurfaceHit &hit) const
{
    Color radiance = Color::black();
    
    // Суммирует яркость от каждого источника света области.
    auto areaLights = scene_->areaLights();
    for(auto areaLight = areaLights.begin(); areaLight != areaLights.end(); areaLight++)
    {
        const Vector3 lightPoint = areaLight->getPoint();
        const Vector3 toLight = lightPoint - hit.position;
        
        if(componentEnabled(RC_Shadows) == false || checkVisibility(hit.position + hit.normal * 0.0001f, lightPoint - toLight * 0.0001f))
        {
            // Вычисление направления и интенсивности входящего света
            const Color lightRadiance = areaLight->emission() / (4.0f * M_PI * toLight.sqrMagnitude());
            
            // Использование блинфонга bsdf
            float ndotl = Vector3::dot(hit.normal, toLight.normalized());
            float lightndotl = Vector3::dot(areaLight->normal(), toLight.normalized() * -1.0f);
            if(ndotl > 0.0f && lightndotl > 0.0f)
            {
                const Vector3 toEye = viewRay.direction() * -1.0f;
                radiance += blinnPhong(hit, toEye, toLight.normalized()) * lightRadiance * ndotl * lightndotl;
            }
        }
    }

    return radiance;
}

Color Renderer::sampleImpulseLight(const Ray &viewRay, const SurfaceHit &hit, int bounce) const
{
    // Находит вектор отражения
    const Vector3 refl = viewRay.direction() - 2.0f * Vector3::dot(viewRay.direction(), hit.normal) * hit.normal;

    // Трассировка пути для поиска импульса в направлении отражения
    return pathTrace(Ray(hit.position, refl), bounce + 1) * hit.reflectivity;
}

Color Renderer::sampleIndirectLight(const SurfaceHit &hit, int bounce) const
{
    if(random_float() > 0.5f) return Color::black();

    // Построение случайного луча в полусфере поверхности и исследование луча в этом направлении.
    const Vector3 dir = random_hemisphere_ray(hit.normal);
    return pathTrace(Ray(hit.position, dir), bounce + 1);
}

SurfaceHit Renderer::testRay(const Ray &ray) const
{
    // Сохранение пути до ближайшей поверхности, которую пересекает луч.
    SurfaceHit hit;
    hit.distance = INFINITY;
    
    // Проверка пересечения каждого треугольника в сцене.
    for(unsigned int i = 0; i < scene_->triangles().size(); ++i)
    {
        // Получение треугольника
        const Triangle triangle = scene_->triangles()[i];
        
        // Проверка пересечения с лучом
        float alpha, beta, gamma, triangleDistance;
        if(ray.intersectionWithTriangle(triangle, alpha, beta, gamma, triangleDistance)
           && triangleDistance < hit.distance)
        {
            // Запись подробностей попадания луча.
            hit.distance = triangleDistance;
            hit.position = ray.origin() + ray.direction() * triangleDistance;
            hit.normal = triangle.normal;
            hit.color = triangle.color;
            hit.emission = triangle.emission;
            hit.reflectivity = triangle.reflectivity;
        }
    }

    if(componentEnabled(RC_Spheres))
    {
        // Проверка пересечения каждой сферы в сцене.
        for(unsigned int i = 0; i < scene_->spheres().size(); ++i)
        {
            // Получение сферы
            const Sphere sphere = scene_->spheres()[i];

            // Проверка пересечения с лучом
            float distance;
            if(ray.intersectionWithSphere(sphere, distance)
                && distance < hit.distance)
            {
                // Запись подробностей попадания луча.
                hit.distance = distance;
                hit.position = ray.origin() + ray.direction() * distance;
                hit.normal = (hit.position - sphere.centre()).normalized();
                hit.color = sphere.color();
                hit.emission = Color::black();
                hit.reflectivity = sphere.reflectivity();
            }
        }
    }
    
    // Возвращение деталей попадания.
    // Если попадания не произошло, расстояние попадания будет INFINITY
    return hit;
}

bool Renderer::checkVisibility(const Vector3 &a, const Vector3 &b) const
{
    // Нахождение направления и расстояния от a до b.
    const Vector3 direction = b - a;
    const float sqrDistance = direction.sqrMagnitude();
    
    // Нахождение расстояния, на которое пройдет луч от a до b.
    const float hitDistance = testRay(Ray(a, direction)).distance;
    const float hitDistanceSqr = hitDistance * hitDistance;
    
    // Проверка, не коснулся ли луч чего-нибудь, прежде чем достичь b.
    return (hitDistanceSqr >= sqrDistance) ? 1.0f : 0.0f;
}

Color Renderer::blinnPhong(const SurfaceHit &surface, const Vector3 &toEye, const Vector3 &toLight) const
{
    Vector3 h = (toLight + toEye).normalized();
    float ndoth = std::max(0.0f, Vector3::dot(surface.normal, h));
    float spec = powf(ndoth, 25.0f) * 0.15f;
    return (surface.color + spec) / M_PI;
}
