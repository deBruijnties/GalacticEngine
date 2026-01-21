#pragma once
#include "Core/Scene/Component.h"


class PointLight : public Component
{
public:
    Vector3 color = Vector3(1,1,1);
    float radius = 10;
    float intensity = 5;

    void OnStart() override;
    void OnUpdate() override;
    void OnSubmitRender() override;
};
