#include <pch.h>
#include "MeshRenderer.h"
#include "core/Scene/Gameobject.h"
#include "core/Scene/Scene.h"
#include "core/Scene/Components/Transform.h"
#include "core/Scene/Components/Camera.h"
#include "core/Rendering/Resources/Material.h"

#include <glm/gtc/matrix_inverse.hpp>
#include "Core/engine.h"
#include "core/Rendering/Mesh/Mesh.h"
#include "core/Rendering/Renderer.h"




void MeshRenderer::OnStart()
{
	transform = gameObject->getComponent<Transform>();
}

void MeshRenderer::OnUpdate()
{
}


void MeshRenderer::OnSubmitRender()
{
	// G-buffer outputs
	// Albedo.rgb (RGBA8) + Metallic.a (A8)
	// Normal.rgb (RGB16F) + Roughness.a (A8)
	// Emission.rgb (RGB16F) + AmbientOcclusion.a (A8)
	// Depth24 / Depth32F (your choice)

	if (material == nullptr || mesh == nullptr)
		return;

	Renderer::RenderCommand cmd;
	cmd.mesh = mesh;
	cmd.material = material;
	cmd.CastsShadow = true;
	cmd.transform = transform->worldMatrix;

	Renderer::Submit(cmd);
}
