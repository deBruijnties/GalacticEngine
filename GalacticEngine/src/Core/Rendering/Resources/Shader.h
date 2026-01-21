#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>


class Shader
{
public:
    Shader(const char* vertexSource, const char* fragmentSource, bool LoadFromFile = false);


    // Override base class
    void bind() const;
    unsigned int getID() const { return ID; }

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector3(const std::string& name, const Vector3& value) const;
    void setMatrix4x4(const std::string& name, const Matrix4& matrix) const;
    static void AddIncludeDirectory(const std::string& path);

private:
    unsigned int ID;
    
    static std::vector<std::string> s_IncludeDirectories;
    static std::string preprocessIncludes(
        const std::string& source,
        const std::string& parentPath
    );
    static std::string getDirectory(const std::string& path);
    unsigned int compileShader(const char* source, unsigned int shaderType);
    void checkCompileErrors(unsigned int shader, const std::string& type);
    static std::string loadFileToString(const std::string& path);
    mutable std::unordered_map<std::string, int> uniformCache;

    int GetUniformLocation(const std::string& name) const;
};
