#pragma once

#include <QElapsedTimer>
#include <atomic>

class RendererStats
{
public:
    RendererStats();
    
    // Получает время, в течение которого выполнялся рендеринг.
    double currentRenderTime() const;

    // Получает общее количество обработанных образцов.
    uint64_t sampleCount() const;

    // Получает среднее количество образцов на пиксель
    uint64_t averageSamplesPerPixel() const;
    
    // Получает среднее количество выборок, выполняемых в секунду.
    uint64_t averageSamplesPerSecond() const;

    // Перезапускает статистику для нового рендера..
    void renderStarted(int width, int height);

    // Добавляет новый образец в счет
    void addSample();
    
private:
    
    // Таймер, используемый для измерения времени рендеринга.
    QElapsedTimer timer_;
    
    // Разрешение изображения
    int width_;
    int height_;

    // Количество готовых образцов.
    std::atomic<std::uint64_t> samplesCount_;
};
