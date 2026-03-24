#include "pch.h"
#include "Camera.h"
#include <Core/Scene/Components/Transform.h>
#include <Core/Rendering/Buffers/framebuffer.h>
#include <Core/Scene/Gameobject.h>

std::vector<Camera*> Camera::allCameras = {};

Camera::Camera()
{
	width = Engine::width;
	height = Engine::height;
    RecalculateProjection();
	allCameras.push_back(this);
	gbuffer.Create(width, height);
    internalOutput.init(width, height, FrameBufferFormat::RGBA16F);
}

void Camera::OnResize(int width, int height)
{
    if (!output)
    {
        this->width = width;
        this->height = height;
        gbuffer.Resize(width, height);
        RecalculateProjection();
        internalOutput.resize(width, height);
    }
}

const Matrix4 Camera::GetViewMatrix() const
{
    return Math::Inverse(transform->worldMatrix);
}

const Matrix4& Camera::GetProjectionMatrix() const
{
    return projection;
}

const Matrix4 Camera::GetViewProjectionMatrix() const
{
    return projection * Math::Inverse(transform->worldMatrix);
}

void Camera::BindInternal()
{
    internalOutput.bind();
}

float Camera::GetFOV() const
{
    return fov;
}

float Camera::GetNearPlane() const
{
    return nearPlane;
}

float Camera::GetFarPlane() const
{
    return farPlane;
}

Vector2Int Camera::GetResolution() const
{
    return Vector2Int(width,height);
}

float Camera::GetAspect() const
{
    return static_cast<float>(width) / static_cast<float>(height);
}

FrameBuffer* Camera::GetOutput()
{
	return output;
}

FrameBuffer* Camera::GetInternalOutput()
{
    return &internalOutput;
}

void Camera::SetFOV(float v)
{
    fov = v;
}

void Camera::SetNearPlane(float v)
{
    nearPlane = v;
}

void Camera::SetFarPlane(float v)
{
    farPlane = v;
}

void Camera::SetOutput(FrameBuffer* fb)
{
	output = fb;
    this->width = fb->width;
    this->height = fb->height;
	gbuffer.Resize(width, height);
    RecalculateProjection();
    internalOutput.resize(width, height);

}

void Camera::RecalculateProjection()
{
    projection = Math::perspective(
        Math::Radians(fov),
        GetAspect(),
        nearPlane,
        farPlane
	);
}