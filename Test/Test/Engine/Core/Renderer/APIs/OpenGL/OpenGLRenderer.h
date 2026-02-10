#pragma once
#include <vector>
#include "core/Renderer/Backend/IRenderer.h"

namespace GalacticEngine
{
    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer();

        void Submit(RenderCommand& cmd) override;

        void Render() override;

        void Release() override;
    private:

        void ClearQueues();

    private:
        // Opaque Queues
        static std::vector<RenderCommand> s_OpaqueQueue;
        static std::vector<RenderCommand> s_OpaqueUnlitQueue;

        // Transparent Queues
        static std::vector<RenderCommand> s_TransparentQueue;
        static std::vector<RenderCommand> s_TransparentUnlitQueue;

        // Shadow Queues
        static std::vector<RenderCommand> s_ShadowQueue;

        // Lighting Queues
        // static StructuredBuffer* s_PointLightBuffer;
    };

}
