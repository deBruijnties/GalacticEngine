#include "OpenGLMesh.h"
#include <iostream>

void GalacticEngine::OpenGLMesh::Draw(uint32_t instanceCount)
{
	std::cout << "Drew OpenGLMesh\n";
}

void GalacticEngine::OpenGLMesh::Upload(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors)
{
	std::cout << "Uploaded OpenGLMesh data\n";
}

void GalacticEngine::OpenGLMesh::Release()
{
	std::cout << "Released OpenGLMesh\n";
}
