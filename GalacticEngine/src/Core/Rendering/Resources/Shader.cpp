#include <pch.h>
#include "Shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// shared across all shaders - engine sets these up once at startup (e.g. "engineassets/shaders/")
std::vector<std::string> Shader::s_IncludeDirectories;


Shader::Shader(const char* vertexCode, const char* fragmentCode, bool LoadFromFile)
{
    std::cout << "Loaded Shader " << fragmentCode << '\n';

    std::string vertexSource;
    std::string fragmentSource;

    const char* vShaderCode = vertexCode;
    const char* fShaderCode = fragmentCode;

    if (LoadFromFile)
    {
        // preprocessIncludes recurses through #include directives and stitches the files together
        vertexSource = preprocessIncludes(
            loadFileToString(vertexCode),
            getDirectory(vertexCode)
        );

        fragmentSource = preprocessIncludes(
            loadFileToString(fragmentCode),
            getDirectory(fragmentCode)
        );

        if (vertexSource.empty() || fragmentSource.empty()) {
            std::cerr << "ERROR::SHADER::FAILED_TO_LOAD_SHADER_FILES\n";
            ID = 0;
            return;
        }

        // important: point at the std::strings above, not the original char* args
        // the strings need to stay in scope until glShaderSource is done with them
        vShaderCode = vertexSource.c_str();
        fShaderCode = fragmentSource.c_str();
    }

    GLuint vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // once linked into the program the individual shader objects are no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind() const
{
    glUseProgram(ID);
}

// uniform setters all follow the same pattern: cache lookup -> set if found
// silently ignores uniforms that don't exist (loc < 0) so unused uniforms don't spam errors

void Shader::setBool(const std::string& name, bool value) const
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1i(static_cast<GLint>(loc), value ? 1 : 0);
}

void Shader::setInt(const std::string& name, int value) const
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1i(static_cast<GLint>(loc), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform1f(static_cast<GLint>(loc), value);
}

void Shader::setVector3(const std::string& name, const Vector3& value) const
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniform3f(static_cast<GLint>(loc), value.x, value.y, value.z);
}

void Shader::setMatrix4x4(const std::string& name, const Matrix4& matrix) const
{
    int loc = GetUniformLocation(name);
    if (loc >= 0)
        glUniformMatrix4fv(static_cast<GLint>(loc), 1, GL_FALSE, matrix.m);
}


void Shader::AddIncludeDirectory(const std::string& path)
{
    if (path.empty())
        return;

    std::string normalized = path;

    // ensure trailing slash so we can blindly concatenate filenames onto it
    if (normalized.back() != '/' && normalized.back() != '\\')
        normalized += '/';

    s_IncludeDirectories.push_back(normalized);
}


std::string Shader::loadFileToString(const std::string& path)
{
    std::ifstream file(path, std::ios::binary); // binary mode so we don't mangle line endings
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    // some editors (especially on Windows) save with a UTF-8 BOM - strip it or the GLSL compiler chokes
    if (data.size() >= 3 &&
        (unsigned char)data[0] == 0xEF &&
        (unsigned char)data[1] == 0xBB &&
        (unsigned char)data[2] == 0xBF)
    {
        data.erase(0, 3);
    }

    return data;
}

// caches uniform locations so we're not calling glGetUniformLocation every frame
// first call is slow, everything after is just a map lookup
int Shader::GetUniformLocation(const std::string& name) const
{
    auto it = uniformCache.find(name);
    if (it != uniformCache.end())
        return it->second;

    GLint loc = glGetUniformLocation(ID, name.c_str());
    uniformCache[name] = static_cast<int>(loc); // -1 if not found, that's fine
    return uniformCache[name];
}


// walks through the source line by line and replaces #include directives with actual file contents
// handles both relative includes ("file.glsl") and system-style includes (<file.glsl>)
// relative includes are resolved from the including file's directory first,
// then falls back to the registered include directories
std::string Shader::preprocessIncludes(const std::string& source, const std::string& parentPath)
{
    std::stringstream output;
    std::istringstream input(source);
    std::string line;

    while (std::getline(input, line))
    {
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace != std::string::npos &&
            line.compare(firstNonSpace, 8, "#include") == 0)
        {
            bool isSystemInclude = line.find('<') != std::string::npos;

            size_t start, end;
            if (isSystemInclude)
            {
                start = line.find('<') + 1;
                end = line.find('>');
            }
            else
            {
                start = line.find('"') + 1;
                end = line.find_last_of('"');
            }

            if (start == std::string::npos || end == std::string::npos || end <= start)
            {
                std::cerr << "ERROR::SHADER::INVALID_INCLUDE_SYNTAX\n";
                continue;
            }

            std::string includeFile = line.substr(start, end - start);
            std::string fullPath;
            bool found = false;

            // try relative to the file that contains this #include
            if (!isSystemInclude)
            {
                fullPath = parentPath + includeFile;
                if (std::ifstream(fullPath).good())
                    found = true;
            }

            // fall back to the global include directories (e.g. engine shader library)
            if (!found)
            {
                for (const auto& dir : s_IncludeDirectories)
                {
                    std::string candidate = dir + includeFile;
                    if (std::ifstream(candidate).good())
                    {
                        fullPath = candidate;
                        found = true;
                        break;
                    }
                }
            }

            if (!found)
            {
                std::cerr << "ERROR::SHADER::INCLUDE_NOT_FOUND: " << includeFile << "\n";
                continue;
            }

            // recurse so included files can have their own #includes
            output << preprocessIncludes(loadFileToString(fullPath), getDirectory(fullPath)) << "\n";
        }
        else
        {
            output << line << "\n";
        }
    }

    return output.str();
}


std::string Shader::getDirectory(const std::string& path)
{
    size_t slash = path.find_last_of("/\\");
    if (slash == std::string::npos)
        return ""; // no directory component, treat as current dir
    return path.substr(0, slash + 1);
}


unsigned int Shader::compileShader(const char* source, unsigned int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type)
{
    GLint  success;
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