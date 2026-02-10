#include <iostream>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Frontend/Mesh.h>
#include <Core/Renderer/Frontend/Shader.h>
#include <Core/Renderer/APIs/OpenGL/OpenGLMaterial.h>
#include <Core/Renderer/Frontend/Material.h>
#include <Core/Window/Window.h>

using GalacticEngine::Mesh;
using GalacticEngine::Shader;
using GalacticEngine::Renderer;
using GalacticEngine::Material;
using GalacticEngine::OpenGLMaterial;
using GalacticEngine::GraphicsApi;

int main()
{
    Window window = Window(400, 400, "TEST MULTI ENGINE");

    Renderer::Innit(GraphicsApi::OpenGL);
    
    Shader* testShader = new Shader("vertex.glsl", "fragment.glsl");
    
    Material testMaterial = Material(testShader);
    
    Mesh testMesh = Mesh();

    
    while (!window.ShouldClose())
    {

    	GalacticEngine::RenderCommand cmd;
    	cmd.mesh = &testMesh;
    	cmd.material = &testMaterial;
    	cmd.CastsShadow = true;

        Renderer::Submit(cmd);
        
        Renderer::Render();

        window.PollEvents();
    }
}
