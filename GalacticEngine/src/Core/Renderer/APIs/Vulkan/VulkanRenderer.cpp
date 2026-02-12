#include "VulkanRenderer.h"
#include <iostream>

namespace GalacticEngine
{
	VulkanRenderer::VulkanRenderer()
	{
		std::cout << "Init VULKAN";
	}

	void VulkanRenderer::Submit(RenderCommand& cmd)
	{
		std::cout << "Submit VULKAN";
	}

	void VulkanRenderer::Render()
	{
		std::cout << "Render VULKAN";
	}

	void VulkanRenderer::Release()
	{
	}

}