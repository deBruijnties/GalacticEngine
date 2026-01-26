#include <pch.h>
#include "MeshRendererInstanced.h"
#include <Core/Rendering/Buffers/StructuredBuffer.h>
#include <Core/engine.h>
#include <Core/Rendering/Resources/Material.h>

#include <Core/Rendering/Renderer.h>

void MeshRendererInstanced::OnSubmitRender()
{

	// G-buffer outputs
	// Albedo.rgb (RGBA8) + Metallic.a (A8)
	// Normal.rgb (RGB16F) + Roughness.a (A8)
	// Emission.rgb (RGB16F) + AmbientOcclusion.a (A8)
	// Depth24 / Depth32F (your choice)

	if (material == nullptr || mesh == nullptr || instanceBuffer == nullptr)
		return;

	Renderer::InstancedRenderCommand cmd;
	cmd.mesh = mesh;
	cmd.material = material;
	cmd.CastsShadow = true;
	cmd.instanceBuffer = instanceBuffer;

	Renderer::SubmitInstanced(cmd);

}
