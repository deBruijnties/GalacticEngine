#include <pch.h>
#include "Texture.h"
#include "stb_image.h"


Texture::Texture()
    : ID(0), Width(0), Height(0), Channels(0)
{
}

Texture::Texture(const std::string& filepath, bool flipVertically)
    : ID(0), Width(0), Height(0), Channels(0)
{
    stbi_set_flip_vertically_on_load(flipVertically);

    // --- Try 16-bit first (TIFF, height maps, spec maps, etc.)
    unsigned short* data16 = stbi_load_16(
        filepath.c_str(),
        &Width,
        &Height,
        &Channels,
        0
    );

    if (data16)
    {
        Generate(data16, GL_UNSIGNED_SHORT);
        stbi_image_free(data16);

        std::cout << "Loaded 16-bit texture: " << filepath
            << " (" << Width << "x" << Height
            << ", channels: " << Channels << ")\n";
        return;
    }

    // --- Fallback to 8-bit
    unsigned char* data8 = stbi_load(
        filepath.c_str(),
        &Width,
        &Height,
        &Channels,
        0
    );

    if (!data8)
    {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        std::cerr << "stb error: " << stbi_failure_reason() << "\n";
        return;
    }

    Generate(data8, GL_UNSIGNED_BYTE);
    stbi_image_free(data8);

    std::cout << "Loaded 8-bit texture: " << filepath
        << " (" << Width << "x" << Height
        << ", channels: " << Channels << ")\n";
}

Texture::~Texture()
{
    if (ID != 0)
        glDeleteTextures(1, &ID);
}
void Texture::Generate(const void* data, GLenum dataType)
{
    GLenum format;
    GLenum internalFormat;

    switch (Channels)
    {
    case 1:
        format = GL_RED;
        internalFormat = (dataType == GL_UNSIGNED_SHORT) ? GL_R16 : GL_R8;
        break;
    case 3:
        format = GL_RGB;
        internalFormat = (dataType == GL_UNSIGNED_SHORT) ? GL_RGB16 : GL_RGB8;
        break;
    case 4:
        format = GL_RGBA;
        internalFormat = (dataType == GL_UNSIGNED_SHORT) ? GL_RGBA16 : GL_RGBA8;
        break;
    default:
        std::cerr << "Unsupported channel count: " << Channels << "\n";
        return;
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        Width,
        Height,
        0,
        format,
        dataType,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::Bind(unsigned int unit) const
{
    if (ID == 0)
    {
        std::cerr << "Attempted to bind uninitialized texture\n";
        return;
    }

    if (unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    {
        std::cerr << "Texture unit " << unit << " out of range!\n";
        return;
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::LoadFromFile(const std::string& filepath, bool flipVertically)
{
    Texture texture(filepath, flipVertically);
    return texture;
}

