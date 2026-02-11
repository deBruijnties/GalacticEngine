#pragma once
#include <string>
#include "core/Math/Types.h"

namespace GalacticEngine
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;
		virtual void SetVec3(const std::string& name, const Vector3& value) = 0;
		virtual void SetMat4(const std::string& name, const Matrix4& value) = 0;
		virtual void SetInt(const std::string& name, const int value) = 0;


		virtual void Load(const std::string& vertexPath, const std::string& fragmentPath) = 0;
		virtual void Release() = 0;
	};
}