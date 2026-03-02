#pragma once
#include "core/Math/Math.h"

class UIElement
{
public:
    virtual ~UIElement() = default;

    // Called every frame to render the element
    virtual void Render() = 0;

    // Position, scale, rotation for the element
    Vector2 Position;
    Vector2 Size;
    float Rotation = 0.0f;
};