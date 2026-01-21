#include "pch.h"
#include "GBuffer.h"
#include <GLAD/glad.h>
#include <iostream>

GBuffer::GBuffer() {}
GBuffer::~GBuffer() { Destroy(); }

bool GBuffer::Create(int width, int height)
{
    Destroy();

    Width = width;
    Height = height;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // 1. Albedo + Metallic  (RGBA8)
    glGenTextures(1, &AlbedoMetalTex);
    glBindTexture(GL_TEXTURE_2D, AlbedoMetalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, AlbedoMetalTex, 0);

    // 2. Normal + Roughness  (RGB16F + A8)
    glGenTextures(1, &NormalRoughTex);
    glBindTexture(GL_TEXTURE_2D, NormalRoughTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalRoughTex, 0);

    // 3. Emission + AO  (RGB16F + A8)
    glGenTextures(1, &EmissionAOTex);
    glBindTexture(GL_TEXTURE_2D, EmissionAOTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, EmissionAOTex, 0);


    // Depth
    glGenTextures(1, &DepthTex);
    glBindTexture(GL_TEXTURE_2D, DepthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTex, 0);

    // Specify active color attachments
    GLenum attachments[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, attachments);

    // Check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "GBuffer incomplete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    std::cout << width << ".x | " << height << ".y \n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void GBuffer::Destroy()
{
    if (AlbedoMetalTex) glDeleteTextures(1, &AlbedoMetalTex);
    if (NormalRoughTex) glDeleteTextures(1, &NormalRoughTex);
    if (EmissionAOTex) glDeleteTextures(1, &EmissionAOTex);
    if (DepthTex)      glDeleteTextures(1, &DepthTex);
    if (FBO)           glDeleteFramebuffers(1, &FBO);

    AlbedoMetalTex = NormalRoughTex = EmissionAOTex = DepthTex = 0;
    FBO = 0;
}

void GBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, Width, Height);

}

void GBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Resize(int width, int height)
{
    if (width == Width && height == Height)
        return;

    Create(width, height);
}

void GBuffer::ClearBuffers(const Vector4& albedoClearColor)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // --------------------------------------------------------
    // 0. Albedo + Metallic
    // RGB = albedo
    // A   = metallic
    // --------------------------------------------------------
    GLfloat albedoMetal[4] = {
        albedoClearColor.r,
        albedoClearColor.g,
        albedoClearColor.b,
        albedoClearColor.a   // metallic
    };
    glClearBufferfv(GL_COLOR, 0, albedoMetal);

    // --------------------------------------------------------
    // 1. Normal + Roughness
    // RGB = normal (default +Z)
    // A   = roughness
    // --------------------------------------------------------
    GLfloat normalRough[4] = {
        0.0f, 0.0f, 1.0f,  // normal
        1.0f              // roughness
    };
    glClearBufferfv(GL_COLOR, 1, normalRough);

    // --------------------------------------------------------
    // 2. Emission + AO
    // RGB = emission
    // A   = AO
    // --------------------------------------------------------
    GLfloat emissionAO[4] = {
        0.0f, 0.0f, 0.0f, // emission
        1.0f             // AO
    };
    glClearBufferfv(GL_COLOR, 2, emissionAO);

    // --------------------------------------------------------
    // Depth
    // --------------------------------------------------------
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth);
}


void GBuffer::BindAlbedoMetalTex(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, GetAlbedoMetalTex());
}

void GBuffer::BindNormalRoughTex(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, GetNormalRoughTex());
}

void GBuffer::BindEmissionAOTex(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, GetEmissionAOTex());
}

void GBuffer::BindDepthTex(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, GetDepthTex());
}
