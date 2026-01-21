#pragma once
#include <memory>
#include <Core/Rendering/Mesh.h>

class UIMesh
{
public:
    // Returns a shared quad mesh (lazy-created)
    static std::shared_ptr<Mesh> GetQuad();

private:
    static std::shared_ptr<Mesh> s_Quad;
};
