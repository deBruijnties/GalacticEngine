#include "pch.h"
#include "Renderer.h"
#include <algorithm>
#include <core/Rendering/Mesh/Mesh.h>
#include <core/Rendering/Mesh/FullScreenQuad.h>
#include <Core/Rendering/Resources/Material.h>
#include <Core/Rendering/buffers/framebuffer.h>
#include <Core/Rendering/buffers/StructuredBuffer.h>
#include <Core/Rendering/Resources/Shader.h>
#include <Core/Scene/Components/Transform.h>
#include <Core/Scene/Components/Camera.h>
#include "Buffers/gbuffer.h"
#include <Core/engine.h>

#include <core/Scene/Gameobject.h>
#include <Core/Debug/Profiler.h>
#include <Core/Time.h>
#include <Core/Utils/printmat4.h>


Camera* Renderer::s_currentCamera = nullptr;
Camera* Renderer::s_currentRenderingCamera = nullptr;

Canvas* Renderer::s_Canvas = nullptr;


std::vector<Renderer::RenderCommand> Renderer::s_OpaqueQueue;
std::vector<Renderer::RenderCommand> Renderer::s_OpaqueUnlitQueue;
std::vector<Renderer::RenderCommand> Renderer::s_TransparentQueue;

std::vector<Renderer::InstancedRenderCommand> Renderer::s_InstancedOpaqueQueue;
std::vector<Renderer::InstancedRenderCommand> Renderer::s_InstancedOpaqueUnlitQueue;
std::vector<Renderer::InstancedRenderCommand> Renderer::s_InstancedTransparentQueue;

std::vector<Renderer::RenderCommand> Renderer::s_ShadowQueue;
std::vector<Renderer::InstancedRenderCommand> Renderer::s_ShadowInstancedQueue;

StructuredBuffer* Renderer::s_PointLightBuffer = nullptr;
uint32_t Renderer::s_PointLightCount = 0;

Shader* Renderer::deferredLightingShader = nullptr;
Shader* ditherShader;
Shader* bloomExtractShader;
Shader* bloomCombineShader;
Shader* bloomBlurShader;

// Bloom framebuffers:
static FrameBuffer* s_BloomBuffers[3] = { nullptr, nullptr, nullptr };
static FrameBuffer* s_BloomCombineOutput = nullptr; // full-res combine target
static int s_BloomWidth = 0;
static int s_BloomHeight = 0;

// Number of blur iterations (each iteration = 1 horizontal + 1 vertical pass)
static constexpr int BLOOM_BLUR_PASSES = 2;

// Threshold above which a pixel is considered "bright enough" to bloom
static constexpr float BLOOM_THRESHOLD = 1.0f;

// How strongly the bloom result is added back onto the scene
static constexpr float BLOOM_INTENSITY = 0.6f;


// Helpers

static void EnsureBloomBuffers(int sceneWidth, int sceneHeight)
{
    // Use half resolution for bloom – cheaper and looks fine after blurring
    int bw = sceneWidth / 2;
    int bh = sceneHeight / 2 ;

    if (bw == s_BloomWidth && bh == s_BloomHeight)
        return; // already the right size

    // Delete old buffers if they exist
    for (int i = 0; i < 3; ++i)
    {
        delete s_BloomBuffers[i];
        s_BloomBuffers[i] = nullptr;
    }
    delete s_BloomCombineOutput;
    s_BloomCombineOutput = nullptr;

    // Half-res: extract + blur ping-pong (no depth needed for post-process)
    for (int i = 0; i < 3; ++i)
    {
        s_BloomBuffers[i] = new FrameBuffer;
        s_BloomBuffers[i]->init(bw, bh,FrameBufferFormat::RGBA16F);
    }


    s_BloomCombineOutput = new FrameBuffer;
    s_BloomCombineOutput->init(sceneWidth, sceneHeight, FrameBufferFormat::RGBA16F);

    s_BloomWidth = bw;
    s_BloomHeight = bh;
}


// Init / Shutdown

void Renderer::Init(int width, int height)
{
    // Setup point light buffer
    s_PointLightBuffer = new StructuredBuffer(
        1, // binding = 1
        sizeof(GPUPointLight)
    );
    deferredLightingShader = new Shader(
        "engineassets/shaders/RendererShaders/Fullscreen.vert",
        "engineassets/shaders/RendererShaders/Lighting/GbufferCombine.frag",
        true
    );
    ditherShader = new Shader(
        "engineassets/shaders/RendererShaders/Fullscreen.vert",
        "engineassets/shaders/RendererShaders/Postprocessing/Dither.frag",
        true
    );

    // Bloom shaders
    bloomExtractShader = new Shader(
        "engineassets/shaders/Bloom/Extract.vert",
        "engineassets/shaders/Bloom/Extract.frag",
        true
    );
    bloomCombineShader = new Shader(
        "engineassets/shaders/Bloom/Combine.vert",
        "engineassets/shaders/Bloom/Combine.frag",
        true
    );
    bloomBlurShader = new Shader(
        "engineassets/shaders/Bloom/Blur.vert",
        "engineassets/shaders/Bloom/Blur.frag",
        true
    );

    constexpr size_t MAX_POINT_LIGHTS = 20;
    s_PointLightBuffer->Allocate(MAX_POINT_LIGHTS);

    // Pre-allocate bloom buffers at the initial resolution
    EnsureBloomBuffers(width, height);
}


// Scene begin / end

void Renderer::BeginScene()
{
    s_PointLightCount = 0;

    s_OpaqueQueue.clear();
    s_OpaqueUnlitQueue.clear();
    s_TransparentQueue.clear();

    s_InstancedOpaqueQueue.clear();
    s_InstancedOpaqueUnlitQueue.clear();
    s_InstancedTransparentQueue.clear();

    s_ShadowQueue.clear();
    s_ShadowInstancedQueue.clear();
}

void Renderer::EndScene()
{
    s_PointLightCount = 0;

    s_OpaqueQueue.clear();
    s_InstancedOpaqueUnlitQueue.clear();
    s_TransparentQueue.clear();

    s_InstancedOpaqueQueue.clear();
    s_InstancedTransparentQueue.clear();

    s_ShadowQueue.clear();
    s_ShadowInstancedQueue.clear();
}


// Camera helpers

void Renderer::SetCamera(Camera* camera)
{
    s_currentCamera = camera;
}

Camera* Renderer::GetCamera()
{
    return s_currentCamera;
}

void Renderer::BeginCamera(const Camera& camera)
{
    s_currentRenderingCamera = const_cast<Camera*>(&camera);

    glEnable(GL_DEPTH_TEST);

    const Matrix4 view = camera.GetViewMatrix();
    const Matrix4 proj = camera.GetProjectionMatrix();
    const Matrix4 viewProj = proj * view;
    const Matrix4 invViewProj = Math::Inverse(viewProj);

    Material::SetGlobalMat4("u_View", view);
    Material::SetGlobalMat4("u_Projection", proj);
    Material::SetGlobalMat4("u_ViewProjection", viewProj);
    Material::SetGlobalMat4("u_InverseViewProjection", invViewProj);
    Material::SetGlobalVec3("u_CameraPos", camera.transform->worldPosition);
    Material::SetGlobalInt("u_PointLightCount", s_PointLightCount);
    Material::SetGlobalFloat("u_Time", Time::timeSinceStartup);
}


// EndCamera  –  full post-process chain including bloom

void Renderer::EndCamera()
{
    // GEOMETRY PASS (GBUFFER)
    {
        glEnable(GL_CULL_FACE);
        PROFILE_SCOPE("Deferred Opaque Pass");
        DefferedOpaquePass();
    }

    // Bind camera's internal HDR framebuffer and blit depth from gbuffer
    {
        PROFILE_SCOPE("Bind Camera Framebuffer and blit depth");

        s_currentRenderingCamera->BindInternal();

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            s_currentRenderingCamera->GetInternalOutput()->fbo
        );

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, s_currentRenderingCamera->gbuffer.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_currentRenderingCamera->GetInternalOutput()->fbo);

        glBlitFramebuffer(
            0, 0,
            s_currentRenderingCamera->GetResolution().x,
            s_currentRenderingCamera->GetResolution().y,
            0, 0,
            s_currentRenderingCamera->GetResolution().x,
            s_currentRenderingCamera->GetResolution().y,
            GL_DEPTH_BUFFER_BIT,
            GL_NEAREST
        );
    }

    // LIGHTING PASS
    {
        PROFILE_SCOPE("Deferred Lighting Pass");
        DefferedLightingPass();
    }

    // FORWARD UNLIT PASS
    {
        PROFILE_SCOPE("Forward Unlit Pass");
        ForwardUnlitPass();
    }

    // FORWARD TRANSPARENT PASS
    {
        PROFILE_SCOPE("Forward Transparent Pass");
        ForwardTransparentPass();
    }

    {
        PROFILE_SCOPE("Bloom Pass");

        const int resX = s_currentRenderingCamera->GetResolution().x;
        const int resY = s_currentRenderingCamera->GetResolution().y;

        // Resize bloom buffers if the camera resolution changed
        EnsureBloomBuffers(resX, resY);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        // EXTRACT 
        glBindFramebuffer(GL_FRAMEBUFFER, s_BloomBuffers[0]->fbo);
        glViewport(0, 0, s_BloomWidth, s_BloomHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        bloomExtractShader->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,
            s_currentRenderingCamera->GetInternalOutput()->colorTex);
        bloomExtractShader->setInt("hdrColor", 0);
        bloomExtractShader->setFloat("threshold", BLOOM_THRESHOLD);

        FullscreenQuad::Get().Draw();

        // PING-PONG BLUR

        bool horizontal = true;
        bool firstIteration = true;

        for (int i = 0; i < BLOOM_BLUR_PASSES * 2; ++i)
        {
            int dst = horizontal ? 1 : 2;
            glBindFramebuffer(GL_FRAMEBUFFER, s_BloomBuffers[dst]->fbo);
            glViewport(0, 0, s_BloomWidth, s_BloomHeight);
            glClear(GL_COLOR_BUFFER_BIT);

            bloomBlurShader->bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,
                firstIteration
                ? s_BloomBuffers[0]->colorTex
                : s_BloomBuffers[horizontal ? 2 : 1]->colorTex);

            bloomBlurShader->setInt("image", 0);
            bloomBlurShader->setBool("horizontal", horizontal);

            FullscreenQuad::Get().Draw();

            horizontal = !horizontal;
            firstIteration = false;
        }

        constexpr int BLUR_RESULT_INDEX = (BLOOM_BLUR_PASSES % 2 == 1) ? 1 : 2;

        // COMBINE
        
        glBindFramebuffer(GL_FRAMEBUFFER, s_BloomCombineOutput->fbo);
        glViewport(0, 0, resX, resY);
        glDisable(GL_BLEND); // shader does the add itself; no GL blending needed

        bloomCombineShader->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,
            s_currentRenderingCamera->GetInternalOutput()->colorTex); // scene
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,
            s_BloomBuffers[BLUR_RESULT_INDEX]->colorTex);             // blurred bloom

        bloomCombineShader->setInt("scene", 0);
        bloomCombineShader->setInt("bloomBlur", 1);
        bloomCombineShader->setFloat("bloomIntensity", BLOOM_INTENSITY);

        FullscreenQuad::Get().Draw();

        glEnable(GL_DEPTH_TEST);
    }

    // FINAL COPY + DITHER PASS TO output framebuffer (or screen)
    {
        PROFILE_SCOPE("Dither / Output Pass");

        int w, h;
        if (s_currentRenderingCamera->GetOutput())
        {
            FrameBuffer* out = s_currentRenderingCamera->GetOutput();
            glBindFramebuffer(GL_FRAMEBUFFER, out->fbo);
            w = out->width;
            h = out->height;
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            w = Engine::width;
            h = Engine::height;
        }

        glViewport(0, 0, w, h);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ditherShader->bind();
        glActiveTexture(GL_TEXTURE0);
        // Read from the combine output (scene + bloom), NOT the raw internal
        // HDR that buffer no longer has bloom added into it.
        glBindTexture(GL_TEXTURE_2D, s_BloomCombineOutput->colorTex);
        ditherShader->setInt("u_HDR", 0);

        FullscreenQuad::Get().Draw();
    }

    // UI PASS  (always rendered to screen / final output)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Engine::width, Engine::height);
    UIPass();
}


// Render Passes
void Renderer::DefferedOpaquePass()
{
    s_currentRenderingCamera->gbuffer.Bind();
    s_currentRenderingCamera->gbuffer.ClearBuffers();

    for (const RenderCommand& cmd : s_OpaqueQueue)
    {
        cmd.material->SetMat4("u_Model", cmd.transform);
        cmd.material->Bind();
        cmd.mesh->Bind();
        cmd.mesh->Draw();
    }

    for (const InstancedRenderCommand& cmd : s_InstancedOpaqueQueue)
    {
        cmd.material->Bind();
        cmd.instanceBuffer->Bind();
        cmd.mesh->Bind();
        cmd.mesh->DrawInstanced(
            static_cast<GLsizei>(cmd.instanceBuffer->GetActiveCount())
        );
    }

    s_currentRenderingCamera->gbuffer.Unbind();
}

void Renderer::DefferedLightingPass()
{
    glDisable(GL_DEPTH_TEST);

    s_PointLightBuffer->Upload();
    s_PointLightBuffer->Bind(); // binding = 1

    deferredLightingShader->bind();
    s_currentRenderingCamera->gbuffer.BindAlbedoMetalTex(0);
    s_currentRenderingCamera->gbuffer.BindNormalRoughTex(1);
    s_currentRenderingCamera->gbuffer.BindEmissionAOTex(2);
    s_currentRenderingCamera->gbuffer.BindDepthTex(3);

    const Matrix4 view = Material::mat4GlobalUniforms["u_View"];
    const Matrix4 proj = Material::mat4GlobalUniforms["u_Projection"];
    const Matrix4 viewProj = proj * view;
    const Matrix4 invViewProj = Math::Inverse(viewProj);

    deferredLightingShader->setInt("u_PointLightCount", s_PointLightCount);
    deferredLightingShader->setMatrix4x4("u_View", view);
    deferredLightingShader->setMatrix4x4("u_Projection", proj);
    deferredLightingShader->setMatrix4x4("u_ViewProjection", viewProj);
    deferredLightingShader->setMatrix4x4("u_InverseViewProjection", invViewProj);
    deferredLightingShader->setVector3("u_CameraPos",
        Material::vec3GlobalUniforms["u_CameraPos"]);

    deferredLightingShader->setInt("gAlbedoMetal", 0);
    deferredLightingShader->setInt("gNormalRough", 1);
    deferredLightingShader->setInt("gEmissionAO", 2);
    deferredLightingShader->setInt("gDepth", 3);

    FullscreenQuad::Get().Draw();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::ForwardUnlitPass()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    for (const RenderCommand& cmd : s_OpaqueUnlitQueue)
    {
        cmd.material->SetMat4("u_Model", cmd.transform);
        cmd.material->Bind();
        cmd.mesh->Bind();
        cmd.mesh->Draw();
    }
    for (const InstancedRenderCommand& cmd : s_InstancedOpaqueUnlitQueue)
    {
        cmd.material->Bind();
        cmd.instanceBuffer->Bind();
        cmd.mesh->Bind();
        cmd.mesh->DrawInstanced(
            static_cast<GLsizei>(cmd.instanceBuffer->GetActiveCount())
        );
    }
}

void Renderer::ForwardTransparentPass()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_TEST);

    s_PointLightBuffer->Bind(); // binding = 1

    std::sort(
        s_TransparentQueue.begin(),
        s_TransparentQueue.end(),
        [](const RenderCommand& a, const RenderCommand& b)
        {
            return a.depth > b.depth;
        }
    );

    for (const RenderCommand& cmd : s_TransparentQueue)
    {
        cmd.material->Bind();
        cmd.material->SetMat4("u_Model", cmd.transform);
        cmd.mesh->Bind();
        cmd.mesh->Draw();
    }

    for (const InstancedRenderCommand& cmd : s_InstancedTransparentQueue)
    {
        cmd.material->Bind();
        cmd.instanceBuffer->Bind();
        cmd.mesh->Bind();
        cmd.mesh->DrawInstanced(
            static_cast<GLsizei>(cmd.instanceBuffer->GetActiveCount())
        );
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void Renderer::SkyboxPass()
{
    // Skybox
}

void Renderer::UIPass()
{
    if (!s_Canvas)
        return;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s_Canvas->Render();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


// Submission

void Renderer::Submit(RenderCommand& cmd)
{
    if (!s_currentCamera)
        return;

    if (cmd.material->isTransparent)
        s_TransparentQueue.push_back(cmd);
    else if (cmd.material->isLit)
        s_OpaqueQueue.push_back(cmd);
    else
        s_OpaqueUnlitQueue.push_back(cmd);

    if (cmd.CastsShadow)
        s_ShadowQueue.push_back(cmd);
}

void Renderer::SubmitInstanced(InstancedRenderCommand& cmd)
{
    if (cmd.material->isTransparent)
        s_InstancedTransparentQueue.push_back(cmd);
    else if (cmd.material->isLit)
        s_InstancedOpaqueQueue.push_back(cmd);
    else
        s_InstancedOpaqueUnlitQueue.push_back(cmd);

    if (cmd.CastsShadow)
        s_ShadowInstancedQueue.push_back(cmd);
}

void Renderer::SubmitPointLight(const Vector3& pos, float radius,
    const Vector3& color, float intensity)
{
    GPUPointLight light;
    light.position = Vector4(pos.x, pos.y, pos.z, radius);
    light.color = Vector4(color.x, color.y, color.z, intensity);
    s_PointLightBuffer->Set(s_PointLightCount++, light);
}