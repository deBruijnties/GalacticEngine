#pragma once
#include <Core/Renderer/RenderCommand.h>


namespace GalacticEngine
{

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void Submit(RenderCommand& cmd) = 0;

		virtual void Render() = 0;

		virtual void Release() = 0;
	};
}