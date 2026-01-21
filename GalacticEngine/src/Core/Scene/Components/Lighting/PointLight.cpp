#include "pch.h"
#include "PointLight.h"
#include <Core/Rendering/Renderer.h>
#include <Core/Scene/Components/Transform.h> 

void PointLight::OnStart()
{
}

void PointLight::OnUpdate()
{
}

void PointLight::OnSubmitRender()
{
    Renderer::SubmitPointLight(transform->worldPosition, radius, color, intensity);
}
