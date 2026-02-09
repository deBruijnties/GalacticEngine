#include "Material.h"
#include "core/Renderer/Renderer.h"
#include "core/Renderer/APIs/OpenGL/OpenGLMaterial.h"
#include <iostream>

GalacticEngine::Material::Material(Shader* shader)
{
	SetShader(shader);
}

GalacticEngine::Material::~Material()
{
	material->Release();
}

void GalacticEngine::Material::SetShader(Shader* s)
{
	shader = s;
}

void GalacticEngine::Material::SetFloat(const std::string& name, float value)
{
}

void GalacticEngine::Material::SetBool(const std::string& name, bool value)
{
}

void GalacticEngine::Material::SetVec3(const std::string& name, const Vector3& value)
{
}

void GalacticEngine::Material::SetMat4(const std::string& name, const Matrix4& value)
{
}

void GalacticEngine::Material::SetInt(const std::string& name, const int value)
{
}

void GalacticEngine::Material::SetGlobalFloat(const std::string& name, float value)
{
}

void GalacticEngine::Material::SetGlobalBool(const std::string& name, bool value)
{
}

void GalacticEngine::Material::SetGlobalVec3(const std::string& name, const Vector3& value)
{
}

void GalacticEngine::Material::SetGlobalMat4(const std::string& name, const Matrix4& value)
{
}

void GalacticEngine::Material::SetGlobalInt(const std::string& name, const int value)
{
}

void GalacticEngine::Material::SetTexture(const std::string& name, const std::string& filePath, bool flip)
{
}

void GalacticEngine::Material::SetExternalTexture(const std::string& name, unsigned int id)
{
}

void GalacticEngine::Material::SetFrameBufferTexture(const std::string& name, FrameBuffer* fb)
{

}

void GalacticEngine::Material::Innit()
{
	switch (Renderer::selectedAPI)
	{
	case GraphicsApi::OpenGL_es:
	case GraphicsApi::OpenGL:
		material = std::unique_ptr<IMaterial>(new OpenGLMaterial());
		break;
	case GraphicsApi::Vulkan:
		//material = std::unique_ptr<IMaterial>(new VulkanMaterial()); TEMP
		std::cout << "Graphics API not Supported yet\n";
		break;
	default:
		std::cout << "Graphics API not Supported\n";
	}
}
