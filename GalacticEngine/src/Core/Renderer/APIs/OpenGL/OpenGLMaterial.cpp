#include "OpenGLMaterial.h"
#include "core/Renderer/APIs/OpenGL/OpenGLShader.h"
#include <iostream>

void GalacticEngine::OpenGLMaterial::Use()
{
	OpenGLShader* glShader = static_cast<OpenGLShader*>(shader->GetBackend()); // raw pointer cast
	glShader->Use();

    // uniforms
    for (auto& kv : floatUniforms)
        glShader->SetFloat(kv.first, kv.second);

    for (auto& kv : boolUniforms)
        glShader->SetInt(kv.first, kv.second ? 1 : 0);

    for (auto& kv : vec3Uniforms)
        glShader->SetVec3(kv.first, kv.second);

    for (auto& kv : mat4Uniforms)
        glShader->SetMat4(kv.first, kv.second);

    for (auto& kv : intUniforms)
        glShader->SetInt(kv.first, kv.second);

}

void GalacticEngine::OpenGLMaterial::Release()
{
	std::cout << "Released OpenGLMaterial\n";
}
