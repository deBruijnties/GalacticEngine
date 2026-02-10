#include "Shader.h"
#include <iostream>
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/APIs/OpenGL/OpenGLShader.h"
#include "Core/Renderer/APIs/Vulkan/VulkanShader.h"

GalacticEngine::Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	Innit();
	shader->Load(vertexPath, fragmentPath);
}

GalacticEngine::Shader::~Shader()
{
	shader->Release();
}

void GalacticEngine::Shader::Innit()
{
	switch (Renderer::GetGraphicsApi())
	{
	case GraphicsApi::OpenGL_es:
	case GraphicsApi::OpenGL:
		shader = std::unique_ptr<IShader>(new OpenGLShader());
		break;
	case GraphicsApi::Vulkan:
		shader = std::unique_ptr<IShader>(new VulkanShader());
		break;
	default:
		std::cout << "Graphics API not Supported\n";
	}
}
