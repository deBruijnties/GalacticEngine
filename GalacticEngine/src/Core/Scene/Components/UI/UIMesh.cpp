#include "UIMesh.h"

std::shared_ptr<Mesh> UIMesh::s_Quad = nullptr;

std::shared_ptr<Mesh> UIMesh::GetQuad()
{
    if (!s_Quad)
    {
        // Quad in UI space (0..1)
        std::vector<glm::vec3> vertices =
        {
            {0.f, 0.f, 0.f}, // bottom-left
            {1.f, 0.f, 0.f}, // bottom-right
            {1.f, 1.f, 0.f}, // top-right
            {0.f, 1.f, 0.f}  // top-left
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            2, 3, 0
        };

        std::vector<glm::vec2> uvs =
        {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f},
            {0.f, 1.f}
        };

        std::vector<glm::vec3> normals =
        {
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<glm::vec4> colors =
        {
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1}
        };

        s_Quad = std::make_shared<Mesh>(
            vertices,
            indices,
            uvs,
            normals,
            colors
        );

        s_Quad->GenerateBuffers(false);
    }

    return s_Quad;
}
