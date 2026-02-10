#pragma once
#include <string>
#include "core/Math/Types.h"

namespace GalacticEngine
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		//virtual void SetFloat(const std::string& name, float value);
		//virtual void SetBool(const std::string& name, bool value);
		//virtual void SetVec3(const std::string& name, const Vector3& value);
		//virtual void SetMat4(const std::string& name, const Matrix4& value);
		//virtual void SetInt(const std::string& name, const int value);


		virtual bool Load(const std::string& vertexPath, const std::string& fragmentPath) = 0;
		virtual void Release() = 0;
	};
}