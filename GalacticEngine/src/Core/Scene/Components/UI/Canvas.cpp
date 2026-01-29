#include "pch.h"
#include "Canvas.h"

void Canvas::OnResize(int Width, int Height)
{
    ReCalculateMatrix();
}

void Canvas::OnStart()
{
    ReCalculateMatrix();
}

void Canvas::ReCalculateMatrix()
{
    int screenWidth = Engine::width;
    int screenHeight = Engine::height;

    if (renderMode == RenderMode::WorldSpace)
    {
        uiProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform->worldMatrix;
    }

    // Screen-space:
    if (scaleMode == ScaleMode::ConstantPixelSize)
    {
        uiProjectionMatrix = Math::ortho(
            0.0f, (float)screenWidth,
            (float)screenHeight, 0.0f,
            -1.0f, 1.0f
        );
        return;
    }
    else // ScaleWithScreenSize
    {
        float sx = (float)screenWidth / referenceWidth;
        float sy = (float)screenHeight / referenceHeight;
        float scale = std::min(sx, sy);

        Matrix4 proj = Math::ortho(
            0.0f, (float)referenceWidth,
            (float)referenceHeight, 0.0f,
            -1.0f, 1.0f
        );

        Matrix4 scaleM = Math::Scale(Matrix4::Identity(), Vector3(scale, scale, 1.0f));

        uiProjectionMatrix = scaleM * proj;
    }
}