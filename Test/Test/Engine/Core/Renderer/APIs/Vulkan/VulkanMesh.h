#pragma once
#include "core/Renderer/Backend/IMesh.h"

namespace GalacticEngine
{
	class VulkanMesh : public IMesh
	{
    public:
        void Draw(uint32_t instanceCount) override;
        void Upload(const std::vector<Vector3>& vertices,
            const std::vector<uint32_t>& indices,
            const std::vector<Vector2>& uvs,
            const std::vector<Vector3>& normals,
            const std::vector<Vector4>& colors,
            const std::vector<Vector3>& tangents,
            bool dynamicBuffers) override;
        void Release() override;
    private:
        //Vulkan Buffer Variables etc.
	};
}


