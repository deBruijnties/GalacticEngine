#include "OpenGLShader.h"
#include <iostream>

bool GalacticEngine::OpenGLShader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::cout << "OpenGLShader Loaded\n";

	return false;
}

void GalacticEngine::OpenGLShader::Release()
{
	std::cout << "OpenGLShader Released\n";
}
