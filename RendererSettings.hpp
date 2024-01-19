#pragma once

// Encodes a combination of render features.
typedef unsigned int RenderFeatureList;

enum RenderResolution
{
    // Рендеринг в полном разрешении
    RS_FullRes = 0,
    
    // Рендеринг в (w/2, h/2)
    RS_HalfRes = 1,
    
    // Рендеринг в (w/4, h/4)
    RS_QuarterRes = 2,
    
    // Рендеринг в (w/8, h/8)
    RS_EighthRes = 3,
    
    // Рендеринг в (w/16, h/16)
    RS_SixteenthRes = 4,
};

enum RenderComponent
{
    // Включено ли прямое освещение
    RC_Direct = 1,
    
    // Включено ли рассеяние импульсов
    RC_Impulse = 2,
    
    // Включено ли непрямое освещение
    RC_Indirect = 4,
    
    // Включены ли теневые лучи
    RC_Shadows = 8,
    
    // Видны ли сферы в середине сцены
    RC_Spheres = 16,
};

typedef unsigned int RenderComponentList;

enum RenderOverlay
{
    RO_None,
    
    // Визуализация пиксельных лучей
    RO_PixelRays,
    
    // Визуализация расстояния до первого попадания
    RO_Distance,
    
    // Визуализация позиции попадания
    RO_Position,
    
    // Визуализация диффузного цвета
    RO_Color,
    
    // Визуализация нормалей
    RO_Normals,
    
    // Визуализация значений выбросов на поверхность
    RO_Emission,
};
