#include "OpenGLMaterial.h"
#include <iostream>

void GalacticEngine::OpenGLMaterial::Use()
{
	std::cout << "Use OpenGLMaterial\n";
}

void GalacticEngine::OpenGLMaterial::Release()
{
	std::cout << "Released OpenGLMaterial\n";
}
