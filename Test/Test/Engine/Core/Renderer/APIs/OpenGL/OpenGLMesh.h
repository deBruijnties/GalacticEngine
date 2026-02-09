#pragma once
#include "core/Renderer/Backend/IMesh.h"

namespace GalacticEngine
{
	class OpenGLMesh : public IMesh
	{
    public:
        void Draw(uint32_t instanceCount) override;
        void Upload(const std::vector<Vector3>& vertices,
            const std::vector<uint32_t>& indices,
            const std::vector<Vector2>& uvs,
            const std::vector<Vector3>& normals,
            const std::vector<Vector4>& colors) override;
        void Release() override;
        
    private:
        unsigned int _vao = 0;
        unsigned int _vbo = 0;
        unsigned int _ebo = 0;
        unsigned int _nbo = 0;
        unsigned int _cbo = 0;
        unsigned int _uvbo = 0;
        unsigned int _tbo = 0;
	};
}


