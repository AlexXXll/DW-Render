#include "Bitmap.hpp"

Bitmap::Bitmap()
    : width_(0),
    height_(0),
    pixels_(nullptr),
    pixelsSRGB_(nullptr),
    sampleSums_(nullptr),
    sampleCounts_(nullptr)
{
    
}

Bitmap::~Bitmap()
{
    delete[] pixels_;
    delete[] pixelsSRGB_;
    delete[] sampleSums_;
    delete[] sampleCounts_;
}

const Color* Bitmap::srgbConvert() const
{
    // Обновление цветового буфера srgb.
    for(int i = 0; i < width_ * height_; ++i)
    {
        pixelsSRGB_[i] = Color::linearToGamma(pixels_[i]);
    }
    
    // Возвращение версии srgb
    return pixelsSRGB_;
}

void Bitmap::resize(int width, int height)
{
    // Удаление всех существующих буферов
    if(pixels_ != nullptr)
    {
        delete[] pixels_;
        delete[] pixelsSRGB_;
        delete[] sampleSums_;
        delete[] sampleCounts_;
    }
    
    // Выделение новых буферов правильного размера.
    pixels_ = new Color[width * height];
    pixelsSRGB_ = new Color[width * height];
    sampleSums_ = new Color[width * height];
    sampleCounts_ = new int[width * height];
    
    // Сохранение разрешения
    width_ = width;
    height_ = height;
}

void Bitmap::clear()
{
    // Установка каждого пикселя черным и каждого счетчика равным 0.
    for(int i = 0; i < width_ * height_; ++i)
    {
        sampleSums_[i].r = 0.0f;
        sampleSums_[i].g = 0.0f;
        sampleSums_[i].b = 0.0f;
        sampleCounts_[i] = 0;
    }
}

void Bitmap::addSample(int x, int y, const Color &color)
{
    const int i = y * width_ + x;
    
    // Добавление в список образцов
    sampleSums_[i] += color;
    sampleCounts_[i] += 1;
    
    // Обновление значения пикселя, чтобы оно было средним из всех значений
    pixels_[i] = sampleSums_[i] / (float)sampleCounts_[i];
}
