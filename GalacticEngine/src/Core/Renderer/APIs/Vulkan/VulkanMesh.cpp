#include "VulkanMesh.h"
#include <iostream>

void GalacticEngine::VulkanMesh::Draw(uint32_t instanceCount)
{
	std::cout << "Drew VulkanMesh\n";
}

void GalacticEngine::VulkanMesh::Upload(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors)
{
	std::cout << "Uploaded VulkanMesh data\n";
}

void GalacticEngine::VulkanMesh::Release()
{
	std::cout << "Released VulkanMesh\n";
}
