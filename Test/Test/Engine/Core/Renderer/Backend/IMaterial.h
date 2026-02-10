#pragma once
#include <string>
#include "core/Renderer/Frontend/Shader.h"
#include "core/Math/Types.h"
#include <unordered_map>

namespace GalacticEngine
{
	class IMaterial
	{
	public:
		


		virtual void Release() = 0;
		Shader* shader = nullptr;
		std::unordered_map<std::string, Matrix4> mat4Uniforms;
		std::unordered_map<std::string, Vector3> vec3Uniforms;
		std::unordered_map<std::string, float> floatUniforms;
		std::unordered_map<std::string, bool> boolUniforms;
		std::unordered_map<std::string, int> intUniforms;
		//std::unordered_map<std::string, TextureSource> textureUniforms;
	};

}