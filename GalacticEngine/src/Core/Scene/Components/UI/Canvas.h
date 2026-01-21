#pragma once
#include <core/Scene/Component.h>
#include <glm/glm.hpp>
#include <Core/Scene/Components/Camera.h>


class Canvas : public Component
{
public:
    enum class RenderMode
    {
        ScreenSpaceOverlay,
        ScreenSpaceCamera,
        WorldSpace
    };

    enum class ScaleMode
    {
        ConstantPixelSize,
        ScaleWithScreenSize,
        ConstantPhysicalSize
    };

    Camera* targetCamera = nullptr;   // Used by ScreenSpaceCamera

    RenderMode renderMode = RenderMode::ScreenSpaceOverlay;
    ScaleMode scaleMode = ScaleMode::ConstantPixelSize;

    const glm::mat4& GetProjection() const;
    const glm::mat4& GetView(const Camera* cam) const;

};
