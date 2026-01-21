#pragma once
#include "core/engine.h"

class GBuffer {
public:
    unsigned int FBO = 0;

    GBuffer();
    ~GBuffer();

    bool Create(int width, int height);
    void Destroy();

    void Bind() const;
    static void Unbind();

    void Resize(int width, int height);

    void ClearBuffers(const Vector4& albedoClearColor = Vector4(0,0,0,0));

    void BindAlbedoMetalTex(unsigned int slot) const;
    void BindNormalRoughTex(unsigned int slot) const;
    void BindEmissionAOTex(unsigned int slot) const;
    void BindDepthTex(unsigned int slot) const;

    unsigned int GetAlbedoMetalTex()  const { return AlbedoMetalTex; }
    unsigned int GetNormalRoughTex()  const { return NormalRoughTex; }
    unsigned int GetEmissionAOTex()   const { return EmissionAOTex; }
    unsigned int GetDepthTex()        const { return DepthTex; }



private:

    unsigned int AlbedoMetalTex = 0;   // Albedo.rgb + Metallic.a
    unsigned int NormalRoughTex = 0;   // Normal.rgb + Roughness.a
    unsigned int EmissionAOTex = 0;    // Emission.rgb + AO.a
    unsigned int DepthTex = 0;         // Depth24 or Depth32F

    int Width = 0;
    int Height = 0;
};