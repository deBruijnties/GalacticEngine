#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <core/Math/Types.h>
#include <core/Renderer/Frontend/Shader.h>
#include <core/Renderer/Backend/IMaterial.h>

struct FrameBuffer; // TEMP

namespace GalacticEngine
{

    class Material
    {
    public:
        bool Opaque = true;
        bool Lit = true;

    public:

        // Constructors
        Material(Shader* shader);

        // Deconstructor
        ~Material();

        void SetShader(Shader* s);

        void SetFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);
        void SetVec3(const std::string& name, const Vector3& value);
        void SetMat4(const std::string& name, const Matrix4& value);
        void SetInt(const std::string& name, const int value);

        void SetTexture(const std::string& name, const std::string& filePath, bool flip = false);
        void SetExternalTexture(const std::string& name, unsigned int id);
        void SetFrameBufferTexture(const std::string& name, FrameBuffer* fb);

        IMaterial* GetBackend();


    private:
        // Innit Backend Material
        void Innit();

        std::unique_ptr<IMaterial> material;
    };


}
