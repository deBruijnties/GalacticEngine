#include <iostream>
#include "Core/Renderer/Renderer.h"
#include <Core/Renderer/Frontend/Mesh.h>
#include <Core/Renderer/Frontend/Shader.h>
#include <Core/Renderer/APIs/OpenGL/OpenGLMaterial.h>
#include <Core/Renderer/Frontend/Material.h>
#include <Core/Renderer/Renderer.h>

using GalacticEngine::Mesh;
using GalacticEngine::Shader;
using GalacticEngine::Renderer;
using GalacticEngine::Material;
using GalacticEngine::OpenGLMaterial;
using GalacticEngine::GraphicsApi;


int main()
{
	Renderer::selectedAPI = GraphicsApi::OpenGL;
    
    {

        Shader* testShader = new Shader("vertex.glsl", "fragment.glsl");

        Material testMaterial = Material(testShader);

        Mesh testMesh = Mesh();

        for (size_t i = 0; i < 10; i++)
        {
			Renderer::RenderCommand cmd;
			cmd.mesh = &testMesh;
			cmd.material = &testMaterial;
			cmd.CastsShadow = true;


            
        }

    }


    while (true);
    {

    }
}
