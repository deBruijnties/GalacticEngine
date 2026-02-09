#include "Mesh.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/APIs/OpenGL/OpenGLMesh.h"
#include "Core/Renderer/APIs/Vulkan/VulkanMesh.h"
#include <iostream>

GalacticEngine::Mesh::Mesh()
{
	Innit();
	//set vectors
	Upload();
}

GalacticEngine::Mesh::Mesh(const std::string Path)
{
	Innit();
	//set vectors
	Upload();
}

GalacticEngine::Mesh::Mesh(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors)
{
	Innit();
	//set vectors
	Upload();
}

GalacticEngine::Mesh::~Mesh()
{
	mesh->Release();
}

void GalacticEngine::Mesh::RecalculateNormals()
{
	std::cout << "RecalculateNormals\n";
}

void GalacticEngine::Mesh::RecalculateTangents()
{
	std::cout << "RecalculateTangents\n";
}

void GalacticEngine::Mesh::RecalculateBounds()
{
	std::cout << "RecalculateBounds\n";
}

void GalacticEngine::Mesh::Draw(unsigned int instanceCount)
{
	if (dirty)
		Upload();
	mesh->Draw(instanceCount);
}

void GalacticEngine::Mesh::Innit()
{
	switch (Renderer::selectedAPI)
	{
	case GraphicsApi::OpenGL_es:
	case GraphicsApi::OpenGL:
		mesh = std::unique_ptr<IMesh>(new OpenGLMesh());
		break;
	case GraphicsApi::Vulkan:
		mesh = std::unique_ptr<IMesh>(new VulkanMesh());
		break;
	default:
		std::cout << "Graphics API not Supported\n";
	}
}

void GalacticEngine::Mesh::Upload()
{
	mesh->Upload(vertices, indices, uvs, normals, colors);
	dirty = false;
}


