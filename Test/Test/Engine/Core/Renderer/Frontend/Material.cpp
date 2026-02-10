#include "Material.h"
#include "core/Renderer/Renderer.h"
#include "core/Renderer/APIs/OpenGL/OpenGLMaterial.h"
#include <iostream>

GalacticEngine::Material::Material(Shader* shader)
{
	std::cout << "Create Material\n";
	Innit();
	SetShader(shader);
}

GalacticEngine::Material::~Material()
{
	material->Release();
}

void GalacticEngine::Material::SetShader(Shader* s)
{
	material->shader = s;
}

void GalacticEngine::Material::SetFloat(const std::string& name, float value)
{
	material->floatUniforms[name] = value;
}

void GalacticEngine::Material::SetBool(const std::string& name, bool value)
{
	material->boolUniforms[name] = value;
}

void GalacticEngine::Material::SetVec3(const std::string& name, const Vector3& value)
{
	material->vec3Uniforms[name] = value;
}

void GalacticEngine::Material::SetMat4(const std::string& name, const Matrix4& value)
{
	material->mat4Uniforms[name] = value;
}

void GalacticEngine::Material::SetInt(const std::string& name, const int value)
{
	material->intUniforms[name] = value;
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

GalacticEngine::IMaterial* GalacticEngine::Material::GetBackend()
{
	return material.get();
}

void GalacticEngine::Material::Innit()
{
	switch (Renderer::GetGraphicsApi())
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
