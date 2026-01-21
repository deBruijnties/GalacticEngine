#include "pch.h"
#include "Material.h"
#include <iostream>
#include <core/Rendering/Resources/Shader.h>
#include <core/Rendering/Resources/Texture.h>
#include <core/Rendering/Buffers/framebuffer.h>


std::unordered_map<std::string, float> Material::floatGlobalUniforms;
std::unordered_map<std::string, bool> Material::boolGlobalUniforms;
std::unordered_map<std::string, Vector3> Material::vec3GlobalUniforms;
std::unordered_map<std::string, Matrix4> Material::mat4GlobalUniforms;
std::unordered_map<std::string, int> Material::intGlobalUniforms;
std::unordered_map<std::string, TextureSource> Material::textureGlobalUniforms;

Material::Material() = default;

Material::Material(Shader* shader)
    : shader(shader)
{ 
}

void Material::SetShader(Shader* s)
{
    shader = s;
}

void Material::SetFloat(const std::string& name, float value)
{
    floatUniforms[name] = value;
}

void Material::SetBool(const std::string& name, bool value)
{
    boolUniforms[name] = value;
}

void Material::SetVec3(const std::string& name, const Vector3& value)
{
    vec3Uniforms[name] = value;
}

void Material::SetMat4(const std::string& name, const Matrix4& value)
{
    mat4Uniforms[name] = value;
}

void Material::SetInt(const std::string& name, const int value)
{
    intUniforms[name] = value;
}

void Material::SetGlobalFloat(const std::string& name, float value)
{
    floatGlobalUniforms[name] = value;
}

void Material::SetGlobalBool(const std::string& name, bool value)
{
    boolGlobalUniforms[name] = value;
}

void Material::SetGlobalVec3(const std::string& name, const Vector3& value)
{
    vec3GlobalUniforms[name] = value;
}

void Material::SetGlobalMat4(const std::string& name, const Matrix4& value)
{
    mat4GlobalUniforms[name] = value;
}

void Material::SetGlobalInt(const std::string& name, const int value)
{
    intGlobalUniforms[name] = value;
}

void Material::SetTexture(const std::string& name, const std::string& filePath, bool flip)
{
    TextureSource src;
    src.type = TextureSource::Type::FileTexture;
    src.ownedTexture = std::make_unique<Texture>(filePath, flip);
    textureUniforms[name] = std::move(src);
}



void Material::SetExternalTexture(const std::string& name, unsigned int id)
{
    TextureSource src;
    src.type = TextureSource::Type::ExternalTexture;
    src.externalID = id;
    textureUniforms[name] = std::move(src);
}

void Material::SetFrameBufferTexture(const std::string& name, FrameBuffer* fb)
{
    TextureSource src;
    src.type = TextureSource::Type::FrameBufferTexture;
    src.fb = fb;
    textureUniforms[name] = std::move(src);
}


void Material::Bind()
{
    if (!shader)
        return;

    //std::cout << "global uniforms: " << (mat4GlobalUniforms.size() + vec3GlobalUniforms.size() + floatGlobalUniforms.size() + boolGlobalUniforms.size() + intGlobalUniforms.size()) <<"\n";

    shader->bind();

	// Global uniforms
    for (auto& kv : mat4GlobalUniforms)
        shader->setMatrix4x4(kv.first, kv.second);

    for (auto& kv : vec3GlobalUniforms)
        shader->setVector3(kv.first, kv.second);

    for (auto& kv : floatGlobalUniforms)
        shader->setFloat(kv.first, kv.second);

    for (auto& kv : boolGlobalUniforms)
        shader->setInt(kv.first, kv.second ? 1 : 0);

    for (auto& kv : intGlobalUniforms)
        shader->setInt(kv.first, kv.second ? 1 : 0);

    // uniforms
    for (auto& kv : floatUniforms)
        shader->setFloat(kv.first, kv.second);

    for (auto& kv : boolUniforms)
        shader->setInt(kv.first, kv.second ? 1 : 0);

    for (auto& kv : vec3Uniforms)
        shader->setVector3(kv.first, kv.second);

    for (auto& kv : mat4Uniforms)
        shader->setMatrix4x4(kv.first, kv.second);

    for (auto& kv : intUniforms)
        shader->setInt(kv.first, kv.second);


    int unit = 0;

    for (auto& kv : textureUniforms)
    {
        const std::string& uniformName = kv.first;
        TextureSource& src = kv.second;

        GLuint texID = 0;

        switch (src.type)
        {
        case TextureSource::Type::FileTexture:
            texID = src.ownedTexture->ID;
            break;

        case TextureSource::Type::ExternalTexture:
            texID = src.externalID;
            break;

        case TextureSource::Type::FrameBufferTexture:
            if (!src.fb)
            {
                std::cout << "Framebuffer texture is NULL for uniform "
                    << uniformName << "\n";
                continue;
            }
            texID = src.fb->colorTex;
            break;
        }

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texID);
        shader->setInt(uniformName, unit);

        unit++;
    }

    for (auto& kv : textureGlobalUniforms)
    {
        const std::string& uniformName = kv.first;
        TextureSource& src = kv.second;

        GLuint texID = 0;

        switch (src.type)
        {
        case TextureSource::Type::FileTexture:
            texID = src.ownedTexture->ID;
            break;

        case TextureSource::Type::ExternalTexture:
            texID = src.externalID;
            break;

        case TextureSource::Type::FrameBufferTexture:
            if (!src.fb)
            {
                std::cout << "Framebuffer texture is NULL for uniform "
                    << uniformName << "\n";
                continue;
            }
            texID = src.fb->colorTex;
            break;
        }

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texID);
        shader->setInt(uniformName, unit);

        unit++;
    }
}
