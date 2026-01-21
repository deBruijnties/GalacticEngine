#pragma once
#include "Camera.h"
#include "../Component.h"
#include <Core/Scene/Gameobject.h>
#include <Core/Rendering/Mesh/Mesh.h>
#include <Core/Rendering/Resources/Material.h>

class StructuredBuffer;

class MeshRendererInstanced : public Component
{
public:

    Mesh* mesh = nullptr;
    Material* material = nullptr;

    StructuredBuffer* instanceBuffer = nullptr;

    void SetInstanceBuffer(StructuredBuffer* buffer)
    {
        instanceBuffer = buffer;
    }

    void OnSubmitRender() override;
};
