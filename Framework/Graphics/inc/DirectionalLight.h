#pragma once
#include "Color.h"

namespace TRengine::Graphics
{
    struct DirectionalLight
    {
        Color ambient = Colors::White;
        Color diffuse = Colors::White;
        Color specular = Colors::White;
        Math::Vector3 direction = Math::Vector3::ZAxis;
        float padding = 0.0f; //to keep 16 byte aligned

    };
}