#include "OpenGLShader.h"
#include <iostream>
#include <GLAD/glad.h>
#include <fstream>
#include <sstream>

void GalacticEngine::OpenGLShader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::cout << "Loaded Shader " << fragmentPath << '\n';
    // Keep strings alive for the lifetime of this constructor
    std::string vertexSource;
    std::string fragmentSource;

    
    vertexSource = loadFileToString(vertexPath);

    fragmentSource = loadFileToString(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "ERROR::SHADER::FAILED_TO_LOAD_SHADER_FILES\n";
        ID = 0;
        return;
    }

    // Use c_str() from strings that are still in scope
    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    // 2. Compile shaders
    GLuint vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

    // 3. Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // 4. Delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}



void GalacticEngine::OpenGLShader::Release()
{
	std::cout << "OpenGLShader Released\n";
}

void GalacticEngine::OpenGLShader::Use() const
{
    glUseProgram(ID);
}

void GalacticEngine::OpenGLShader::SetFloat(const std::string& name, float value)
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1f(static_cast<GLint>(loc), value);
}

void GalacticEngine::OpenGLShader::SetBool(const std::string& name, bool value)
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1i(static_cast<GLint>(loc), value ? 1 : 0);
}

void GalacticEngine::OpenGLShader::SetVec3(const std::string& name, const Vector3& value)
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform3f(
            static_cast<GLint>(loc),
            value.x, value.y, value.z
        );
}

void GalacticEngine::OpenGLShader::SetMat4(const std::string& name, const Matrix4& value)
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniformMatrix4fv(
            static_cast<GLint>(loc),
            1,
            GL_FALSE,
            value.m
        );
}

void GalacticEngine::OpenGLShader::SetInt(const std::string& name, const int value)
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1i(static_cast<GLint>(loc), value);
}



unsigned int GalacticEngine::OpenGLShader::compileShader(const char* source, unsigned int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void GalacticEngine::OpenGLShader::checkCompileErrors(unsigned int shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
    else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}

std::string GalacticEngine::OpenGLShader::loadFileToString(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    // Remove UTF-8 BOM if present (EF BB BF)
    if (data.size() >= 3 &&
        (unsigned char)data[0] == 0xEF &&
        (unsigned char)data[1] == 0xBB &&
        (unsigned char)data[2] == 0xBF)
    {
        data.erase(0, 3);
    }

    return data;
}

int GalacticEngine::OpenGLShader::GetUniformLocation(const std::string& name) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    return loc;
}

