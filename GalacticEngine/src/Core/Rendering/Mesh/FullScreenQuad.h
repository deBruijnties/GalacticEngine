#pragma once
#include "Mesh.h"

class FullscreenQuad
{
public:
    static Mesh& Get()
    {
        static Mesh quad = Mesh::LoadMeshFromFile("engineassets/models/FullscreenQuad.obj");
        static bool initialized = false;

        if (!initialized)
        {
            quad.GenerateBuffers();
            initialized = true;
        }

        return quad;
    }
};
