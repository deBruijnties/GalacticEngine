#pragma once
#include "core/UI/Canvas.h"
#include <vector>
#include "Buffers/gbuffer.h"

class StructuredBuffer;
class Mesh;
class Material;
class Shader;
class Camera;


class Renderer
{
public:


    struct RenderCommand
    {
        Mesh* mesh;
        Material* material;
        Matrix4 transform;
        float depth; // optional, for sorting
        bool CastsShadow;
    };

    struct InstancedRenderCommand
    {
        Mesh* mesh;
        Material* material;
        StructuredBuffer* instanceBuffer;
        bool CastsShadow;
    };

    struct GPUPointLight
    {
        Vector4 position; // xyz + radius
        Vector4 color;    // rgb + intensity
    };


    static void Init(int width, int height);


    static void BeginScene();
    static void BeginCamera(const Camera& camera);

    static void Submit(RenderCommand& cmd);
    static void SubmitInstanced(InstancedRenderCommand& cmd);
    static void SubmitPointLight(const Vector3& pos, float radius, const Vector3& color, float intensity);

    static void EndCamera();
    static void EndScene();

    static void SetCamera(Camera* camera);
    static Camera* GetCamera();

    static void SetCanvas(Canvas* canvas) { s_Canvas = canvas; }

private:
    static Canvas* s_Canvas; // <-- store the UI Canvas
    
    static void DefferedOpaquePass();
    static void DefferedLightingPass();
    static void ForwardUnlitPass();
    static void ForwardTransparentPass();
    static void SkyboxPass();
    static void UIPass();

	static Camera* s_currentCamera; // cam displayed to screen
	static Camera* s_currentRenderingCamera; // cam being currently rendered


    static std::vector<RenderCommand> s_OpaqueQueue;
    static std::vector<RenderCommand> s_OpaqueUnlitQueue;
    static std::vector<RenderCommand> s_TransparentQueue;

    static std::vector<InstancedRenderCommand> s_InstancedOpaqueQueue;
    static std::vector<InstancedRenderCommand> s_InstancedOpaqueUnlitQueue;
    static std::vector<InstancedRenderCommand> s_InstancedTransparentQueue;

    static std::vector<RenderCommand> s_ShadowQueue;
    static std::vector<InstancedRenderCommand> s_ShadowInstancedQueue;

    static StructuredBuffer* s_PointLightBuffer;
    static uint32_t s_PointLightCount;

    static Shader* deferredLightingShader;
};
