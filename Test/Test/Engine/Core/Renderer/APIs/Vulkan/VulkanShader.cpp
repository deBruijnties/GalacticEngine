#include "VulkanShader.h"
#include <iostream>

bool GalacticEngine::VulkanShader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::cout << "VulkanShader Loaded\n";

	return false;
}

void GalacticEngine::VulkanShader::Release()
{
	std::cout << "VulkanShader Released\n";
}
