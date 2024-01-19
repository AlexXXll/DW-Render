#pragma once

#include "Color.hpp"

class Bitmap
{
public:
    Bitmap();
    ~Bitmap();
    
    // Предотвращение копирования растровых изображений
    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;
    
    int width() const { return width_; }
    int height() const { return height_; }
    const Color* buffer() { return pixels_; }
    
    // Возвращаение версии растрового изображения, закодированного как значения srgb
    const Color* srgbConvert() const;
    
    // Изменение размера и очищение изображения
    void resize(int width, int height);
    
    // Удаление всех образцов и заполнение каждого пикселя черным
    void clear();
    
    // Добавление дополнительного образца в указанное место.
    void addSample(int x, int y, const Color &color);
    
private:
    int width_;
    int height_;
    Color* pixels_;
    Color* pixelsSRGB_;
    Color* sampleSums_;
    int* sampleCounts_;
};
