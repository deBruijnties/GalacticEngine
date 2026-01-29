#pragma once
#include "core/Scene/Component.h"
#include "Core/Rendering/Buffers/gbuffer.h"
#include "Core/Rendering/Buffers/framebuffer.h"

#include <vector>

class Camera : public Component
{
public:
    Camera();

    static std::vector<Camera*> allCameras;

    void OnResize(int width, int height) override;

    // Matrices
    const Matrix4& GetViewMatrix() const;
    const Matrix4& GetProjectionMatrix() const;
    const Matrix4& GetViewProjectionMatrix() const;

    void BindInternal();

    // Projection params
    float GetFOV() const;
    float GetNearPlane() const;
    float GetFarPlane() const;
    Vector2Int GetResolution() const;
    float GetAspect() const;
    FrameBuffer* GetOutput();
    FrameBuffer* GetInternalOutput();

    void SetFOV(float v);
    void SetNearPlane(float v);
    void SetFarPlane(float v);
    void SetOutput(FrameBuffer* fb);

    GBuffer gbuffer;

	Vector3 clearColor = Vector3(0,0,0);

private:
    void RecalculateProjection();

private:
    FrameBuffer* output = nullptr;

    FrameBuffer internalOutput;

    Matrix4 projection = Matrix4::Identity();

    int width = 0;
    int height = 0;

    float fov = 90.0f;
    float nearPlane = 0.1f;
    float farPlane = 2000.0f;
};
