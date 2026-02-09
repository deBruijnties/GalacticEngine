#pragma once
#include "core/Renderer/Backend/IShader.h"
#include <string>

namespace GalacticEngine
{

	class OpenGLShader : public IShader
	{
	public:
		bool Load(const std::string& vertexPath, const std::string& fragmentPath) override;
		void Release() override;
	private:

	};

}