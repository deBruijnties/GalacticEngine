#include "Renderer.h"
#include <iostream>
#include "core/Renderer/APIs/OpenGL/OpenGLRenderer.h"
#include "core/Renderer/Backend/IRenderer.h"

namespace GalacticEngine
{
    GraphicsApi Renderer::selectedAPI = GraphicsApi::OpenGL;
    std::unique_ptr<GalacticEngine::IRenderer> GalacticEngine::Renderer::renderer = nullptr;

    void Renderer::Innit(GraphicsApi api)
    {
        selectedAPI = api;
        switch (selectedAPI)
        {
        case GraphicsApi::OpenGL_es:
        case GraphicsApi::OpenGL:
            renderer = std::unique_ptr<IRenderer>(new OpenGLRenderer());
            break;
        case GraphicsApi::Vulkan:
            //renderer = std::unique_ptr<IShader>(new VulkanShader());
            std::cout << "Graphics API not Supported YET\n";
            break;
        default:
            std::cout << "Graphics API not Supported\n";
        }
    }

    void Renderer::Submit(RenderCommand& cmd)
    {
        renderer->Submit(cmd);
    }

    void Renderer::Render()
    {
        renderer->Render();
    }

    GraphicsApi Renderer::GetGraphicsApi()
    {
        return selectedAPI;
    }
}