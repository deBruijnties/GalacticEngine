#pragma once
#include <core/Scene/Component.h>
#include <Core/Rendering/Material.h>

class UIImage : public Component
{
public:
    Material* material = nullptr;

protected:
    void OnRender() override;
};
