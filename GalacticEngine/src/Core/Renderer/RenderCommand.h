#pragma once
#include "core/Renderer/Frontend/Material.h"
#include "core/Renderer/Frontend/Mesh.h"
#include "core/Math/Types.h"

namespace GalacticEngine
{
    struct RenderCommand
    {
        Mesh* mesh;
        Material* material;
        Matrix4 transform;
        float depth; // optional, for sorting
        bool CastsShadow;
    };

    //struct InstancedRenderCommand
    //{
    //    Mesh* mesh;
    //    Material* material;
    //    StructuredBuffer* instanceBuffer;
    //    bool CastsShadow;
    //};

    //struct GPUPointLight
    //{
    //    Vector4 position; // xyz + radius
    //    Vector4 color;    // rgb + intensity
    //};
}