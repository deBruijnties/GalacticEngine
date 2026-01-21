#pragma once
#include "Core/Scene/Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
    Transform* transform = nullptr;
    Mesh* mesh = nullptr;
    Material* material = nullptr;

    void OnStart() override;
    void OnUpdate() override;
    void OnSubmitRender() override;
};


