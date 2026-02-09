#pragma once
#include <vector>
#include "core/Renderer/GraphicsAPI.h"
#include "core/Renderer/Frontend/Material.h"
#include "core/Renderer/Frontend/Mesh.h"

namespace GalacticEngine
{
	class Renderer
	{
	public:
		static GraphicsApi selectedAPI;

        struct RenderCommand
        {
            Mesh* mesh;
            Material* material;
            Matrix4 transform;
            bool CastsShadow;
        };

        //struct InstancedRenderCommand
        //{
        //    Mesh* mesh;
        //    Material* material;
        //    StructuredBuffer* instanceBuffer;
        //    bool CastsShadow;
        //};

        struct GPUPointLight
        {
            Vector4 position; // xyz + radius
            Vector4 color;    // rgb + intensity
        };


        static void Submit(RenderCommand& cmd);

        static std::vector<RenderCommand> s_OpaqueQueue;
        static std::vector<RenderCommand> s_OpaqueUnlitQueue;
        static std::vector<RenderCommand> s_TransparentQueue;

        static std::vector<RenderCommand> s_ShadowQueue;




	};
}

