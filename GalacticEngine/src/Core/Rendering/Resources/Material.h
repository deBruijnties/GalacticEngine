#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <core/Rendering/Resources/Texture.h>


class Shader;
class Texture;
class FrameBuffer;

struct TextureSource
{
    enum class Type { FileTexture, ExternalTexture, FrameBufferTexture };

    Type type = Type::FileTexture;

    std::unique_ptr<Texture> ownedTexture; // owned
    unsigned int externalID = 0;           // not owned
    FrameBuffer* fb = nullptr;             // not owned
};

class Material
{
public:
    bool isTransparent = false;
    bool isLit = true;
    Shader* shader = nullptr;

    std::unordered_map<std::string, float> floatUniforms;
    std::unordered_map<std::string, bool> boolUniforms;
    std::unordered_map<std::string, Vector3> vec3Uniforms;
    std::unordered_map<std::string, Matrix4> mat4Uniforms;
    std::unordered_map<std::string, int> intUniforms;
    std::unordered_map<std::string, TextureSource> textureUniforms;

    static std::unordered_map<std::string, float> floatGlobalUniforms;
    static std::unordered_map<std::string, bool> boolGlobalUniforms;
    static std::unordered_map<std::string, Vector3> vec3GlobalUniforms;
    static std::unordered_map<std::string, Matrix4> mat4GlobalUniforms;
    static std::unordered_map<std::string, int> intGlobalUniforms;
    static std::unordered_map<std::string, TextureSource> textureGlobalUniforms;

public:
    Material(); 
    explicit Material(Shader* shader);

    void SetShader(Shader* s);

    void SetFloat(const std::string& name, float value);
    void SetBool(const std::string& name, bool value);
    void SetVec3(const std::string& name, const Vector3& value);
    void SetMat4(const std::string& name, const Matrix4& value);
    void SetInt(const std::string& name, const int value);

    static void SetGlobalFloat(const std::string& name, float value);
    static void SetGlobalBool(const std::string& name, bool value);
    static void SetGlobalVec3(const std::string& name, const Vector3& value);
    static void SetGlobalMat4(const std::string& name, const Matrix4& value);
    static void SetGlobalInt(const std::string& name, const int value);

    void SetTexture(const std::string& name, const std::string& filePath, bool flip = false);
    void SetExternalTexture(const std::string& name, unsigned int id);
    void SetFrameBufferTexture(const std::string& name, FrameBuffer* fb);

    void Bind();
};
