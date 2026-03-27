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
    stbi_set_flip_vertically_on_load(flipVertically); // OpenGL expects UVs bottom-left, most images are top-left

    // try 16-bit first - stb will return null if it's not actually a 16-bit file, so this is safe to attempt
    // useful for TIFFs, heightmaps, spec maps, anything that needs more than 256 shades per channel
    unsigned short* data16 = stbi_load_16(filepath.c_str(), &Width, &Height, &Channels, 0);
    if (data16)
    {
        Generate(data16, GL_UNSIGNED_SHORT);
        stbi_image_free(data16);
        std::cout << "Loaded 16-bit texture: " << filepath
            << " (" << Width << "x" << Height
            << ", channels: " << Channels << ")\n";
        return;
    }

    // 16-bit failed, fall back to standard 8-bit (PNG, JPG, BMP, etc.)
    unsigned char* data8 = stbi_load(filepath.c_str(), &Width, &Height, &Channels, 0);
    if (!data8)
    {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        std::cerr << "stb error: " << stbi_failure_reason() << "\n"; // usually "unknown image type" or "can't fopen"
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
        glDeleteTextures(1, &ID); // no-op safe to skip if never uploaded, but check anyway
}

void Texture::Generate(const void* data, GLenum dataType)
{
    GLenum format;
    GLenum internalFormat;

    // pick the right GL format based on how many channels stb gave us
    // internal format preserves bit depth (R16 vs R8, etc.) so we don't silently downgrade 16-bit data
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
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, Width, Height, 0, format, dataType, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // NOTE: these two lines are immediately overwritten below - the LINEAR_MIPMAP_LINEAR never takes effect
    // either remove the NEAREST lines if you want proper mipmapped filtering,
    // or remove these two if you intentionally want nearest-neighbor (pixel art, etc.)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // <-- overwrites the line above
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // <-- overwrites the line above

    glGenerateMipmap(GL_TEXTURE_2D); // mipmaps are generated but won't be sampled while filter is NEAREST
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

// convenience wrapper so callers don't have to construct a Texture manually
Texture Texture::LoadFromFile(const std::string& filepath, bool flipVertically)
{
    return Texture(filepath, flipVertically);
}