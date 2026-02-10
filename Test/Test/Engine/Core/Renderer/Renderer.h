#pragma once
#include <vector>
#include <memory>
#include "core/Renderer/GraphicsAPI.h"
#include "core/Renderer/Backend/IRenderer.h"
#include "core/Renderer/RenderCommand.h"

namespace GalacticEngine
{
	class Renderer
	{
	public:

        static void Innit(GraphicsApi api = GraphicsApi::OpenGL);
        static void Submit(RenderCommand& cmd);

		static std::unique_ptr<IRenderer> renderer;

		static GraphicsApi GetGraphicsApi();

		static void Render();

	private:
		static GraphicsApi selectedAPI;
	};
}

