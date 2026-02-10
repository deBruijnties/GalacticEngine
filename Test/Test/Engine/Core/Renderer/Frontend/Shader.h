#pragma once
#include "core/Renderer/Backend/IShader.h"
#include <memory>

namespace GalacticEngine
{

	class Shader
	{
	public:
		// Constructors
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		// Deconstructor
		~Shader();

		IShader* GetBackend();
	private:
		// Innit Backend Shader
		void Innit();
	private:

		std::unique_ptr<IShader> shader;
	};

}