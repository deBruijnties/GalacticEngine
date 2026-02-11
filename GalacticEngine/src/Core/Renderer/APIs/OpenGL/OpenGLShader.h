#pragma once
#include "core/Renderer/Backend/IShader.h"
#include <string>
#include <vector>

namespace GalacticEngine
{

	class OpenGLShader : public IShader
	{
	public:
		void Load(const std::string& vertexPath, const std::string& fragmentPath) override;
        void Release() override;
        
        void Use() const;

		void SetFloat(const std::string& name, float value) override;
		void SetBool(const std::string& name, bool value) override;
		void SetVec3(const std::string& name, const Vector3& value) override;
		void SetMat4(const std::string& name, const Matrix4& value) override;
		void SetInt(const std::string& name, const int value) override;
	private:
		unsigned int ID;

        unsigned int compileShader(const char* source, unsigned int shaderType);
        void checkCompileErrors(unsigned int shader, const std::string& type);
        static std::string loadFileToString(const std::string& path);

        int GetUniformLocation(const std::string& name) const;
	};

}