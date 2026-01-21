#pragma once

class Texture
{
public:
    int Width, Height, Channels;
    unsigned int ID;

    Texture();
    Texture(const std::string& filepath, bool flipVertically = true);
    ~Texture();

    void Bind(unsigned int unit = 0) const;
    void Unbind() const;

    static Texture LoadFromFile(const std::string& filepath, bool flipVertically = true);




private:
    void Generate(const void* data, unsigned int dataType);
};
