#pragma once
#include "core/Renderer/Backend/IShader.h"
namespace GalacticEngine
{	

	class VulkanShader : public IShader
	{
		public:
		void Load(const std::string& vertexPath, const std::string& fragmentPath) override;
		void Release() override;
	private:

	};

}