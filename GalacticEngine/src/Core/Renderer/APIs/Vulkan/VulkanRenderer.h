#pragma once
#include <vector>
#include "core/Renderer/Backend/IRenderer.h"

namespace GalacticEngine
{
    class VulkanRenderer : public IRenderer
    {
    public:
        VulkanRenderer();

        void Submit(RenderCommand& cmd) override;

        void Render() override;

        void Release() override;
    private:

    };

}
