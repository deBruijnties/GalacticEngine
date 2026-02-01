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

#include <core/Rendering/Buffers/framebuffer.h>
#include <core/Scene/Gameobject.h>
#include <Core/Debug/Profiler.h>
#include <Core/Time.h>
#include <Core/Utils/printmat4.h>

//#define debugRenderingFence

Camera* Renderer::s_currentCamera = nullptr;
Camera* Renderer::s_currentRenderingCamera = nullptr;

Shader* ditherShader;

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

void Renderer::Init(int width, int height)
{
	// setup point light buffer
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
    

    constexpr size_t MAX_POINT_LIGHTS = 256;
    s_PointLightBuffer->Allocate(MAX_POINT_LIGHTS);
}


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

    // Cleanup
    s_PointLightCount = 0;

    s_OpaqueQueue.clear();
    s_InstancedOpaqueUnlitQueue.clear();
    s_TransparentQueue.clear();

    s_InstancedOpaqueQueue.clear();
    s_InstancedTransparentQueue.clear();

    s_ShadowQueue.clear();
    s_ShadowInstancedQueue.clear();
}

void Renderer::SetCamera( Camera* camera)
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

	// set global uniforms camera data
    Material::SetGlobalMat4("u_View", view);
    Material::SetGlobalMat4("u_Projection", proj);
    Material::SetGlobalMat4("u_ViewProjection", viewProj);
    Material::SetGlobalMat4("u_InverseViewProjection", invViewProj);
    Material::SetGlobalVec3("u_CameraPos", camera.transform->worldPosition);
    Material::SetGlobalInt("u_PointLightCount", s_PointLightCount);
    Material::SetGlobalFloat("u_Time", Time::timeSinceStartup);

}




void Renderer::EndCamera()
{
    // GEOMETRY PASS (GBUFFER)
    {
        glEnable(GL_CULL_FACE);

        PROFILE_SCOPE("Deferred Opaque Pass");
        DefferedOpaquePass();
    }

    // SET CAMERA OUTPUT
    {
        PROFILE_SCOPE("Bind Camera Framebufffer and blit depth");

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
            0, 0, s_currentRenderingCamera->GetResolution().x, s_currentRenderingCamera->GetResolution().y,
            0, 0, s_currentRenderingCamera->GetResolution().x, s_currentRenderingCamera->GetResolution().y,
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
    




    UIPass();






    // FINAL Copy + DITHER PASS

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

    // Clear
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    ditherShader->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_currentRenderingCamera->GetInternalOutput()->colorTex);
    //glBindTexture(GL_TEXTURE_2D, s_currentRenderingCamera->gbuffer.GetNormalRoughTex());//TEMP TEMP TEMP
    ditherShader->setInt("u_HDR", 0);

    FullscreenQuad::Get().Draw();
}


// Passes
void Renderer::DefferedOpaquePass()
{

    // Bind gbuffers
    s_currentRenderingCamera->gbuffer.Bind();
    s_currentRenderingCamera->gbuffer.ClearBuffers(); //clears gbuffers to standard values ALSO BINDS

    for (const RenderCommand& cmd : s_OpaqueQueue)
    {
        cmd.material->SetMat4("u_Model", cmd.transform);
        cmd.material->Bind();
        cmd.mesh->Bind();
        cmd.mesh->Draw();
        //draw to gbuffers
    }

    for (const InstancedRenderCommand& cmd : s_InstancedOpaqueQueue)
    {
        cmd.material->Bind();

        cmd.instanceBuffer->Bind();
        cmd.mesh->Bind();
        cmd.mesh->DrawInstanced(
            static_cast<GLsizei>(
                cmd.instanceBuffer->GetActiveCount()
                )
        );
        //also draw to gbuffers
    }

    s_currentRenderingCamera->gbuffer.Unbind();
}

void Renderer::DefferedLightingPass()
{

    glDisable(GL_DEPTH_TEST);

	// send point light data to GPU
    s_PointLightBuffer->Upload();
    s_PointLightBuffer->Bind(); // binding = 1

    // Draw fullscreen lighting pass
    //glDisable(GL_DEPTH_TEST);

    deferredLightingShader->bind();
    s_currentRenderingCamera->gbuffer.BindAlbedoMetalTex(0);
    s_currentRenderingCamera->gbuffer.BindNormalRoughTex(1);
    s_currentRenderingCamera->gbuffer.BindEmissionAOTex(2);
    s_currentRenderingCamera->gbuffer.BindDepthTex(3);


    const Matrix4 view = Material::mat4GlobalUniforms["u_View"];
    const Matrix4 proj = Material::mat4GlobalUniforms["u_Projection"];
    const Matrix4 viewProj = proj * view;
    const Matrix4 invViewProj = Math::Inverse(viewProj);


    // set global uniforms camera data
    deferredLightingShader->setInt("u_PointLightCount", s_PointLightCount);
    deferredLightingShader->setMatrix4x4("u_View", view);
    deferredLightingShader->setMatrix4x4("u_Projection", proj);
    deferredLightingShader->setMatrix4x4("u_ViewProjection", viewProj);
    deferredLightingShader->setMatrix4x4("u_InverseViewProjection", invViewProj);
    deferredLightingShader->setVector3("u_CameraPos", Material::vec3GlobalUniforms["u_CameraPos"]);

    deferredLightingShader->setInt("gAlbedoMetal", 0);
    deferredLightingShader->setInt("gNormalRough", 1);
    deferredLightingShader->setInt("gEmissionAO", 2);
    deferredLightingShader->setInt("gDepth", 3);

    FullscreenQuad::Get().Draw();

    glEnable(GL_DEPTH_TEST);

}

void Renderer::ForwardUnlitPass()
{
    
    // Opaque unlit forward pass
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);
    
    // No lighting needed, but some unlit shaders might still
    // read camera globals (View/Projection), which are already set

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

    // Transparent
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
            static_cast<GLsizei>(
                cmd.instanceBuffer->GetActiveCount()
                )
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
  
}




// Submission

void Renderer::Submit(RenderCommand& cmd)
{
    // camera-space depth
    if (!s_currentCamera)
        return;
    const Vector3 cameraPos = s_currentCamera->transform->worldPosition;

    if (cmd.material->isTransparent)
        s_TransparentQueue.push_back(cmd);
    else if(cmd.material->isLit)
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

void Renderer::SubmitPointLight(const Vector3& pos, float radius, const Vector3& color, float intensity)
{
    GPUPointLight light;
    light.position = Vector4(pos.x, pos.y, pos.z, radius);
    light.color = Vector4(color.x, color.y, color.z, intensity);
    
    s_PointLightBuffer->Set(s_PointLightCount++, light);
}



