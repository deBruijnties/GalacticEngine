#include "Renderer.h"

namespace GalacticEngine
{

    std::vector<Renderer::RenderCommand> Renderer::s_OpaqueQueue;
    std::vector<Renderer::RenderCommand> Renderer::s_OpaqueUnlitQueue;
    std::vector<Renderer::RenderCommand> Renderer::s_TransparentQueue;
	std::vector<Renderer::RenderCommand> Renderer::s_ShadowQueue;

    GraphicsApi Renderer::selectedAPI = GraphicsApi::OpenGL;

    void Renderer::Submit(RenderCommand& cmd)
    {
		// Shadow casting queue
        if (cmd.CastsShadow)
        {
            s_ShadowQueue.push_back(cmd);
        }
        // Opaque and Transparent queue
        if (cmd.material->Lit)
        {
            if (cmd.material->Opaque)
            {
                s_OpaqueQueue.push_back(cmd);
            }
            else
            {
                s_TransparentQueue.push_back(cmd);
            }
        }
        // Unlit queue
        else 
        {
            s_OpaqueUnlitQueue.push_back(cmd);
		}
    }


}