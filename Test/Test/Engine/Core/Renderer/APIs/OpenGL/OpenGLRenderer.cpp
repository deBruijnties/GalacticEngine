#include "OpenGLRenderer.h"
#include <iostream>

namespace GalacticEngine
{
	std::vector<RenderCommand> OpenGLRenderer::s_OpaqueQueue;
	std::vector<RenderCommand> OpenGLRenderer::s_OpaqueUnlitQueue;

	// Transparent Queues
	std::vector<RenderCommand> OpenGLRenderer::s_TransparentQueue;
	std::vector<RenderCommand> OpenGLRenderer::s_TransparentUnlitQueue;

	// Shadow Queues
	std::vector<RenderCommand> OpenGLRenderer::s_ShadowQueue;



	void OpenGLRenderer::Init(int width, int height)
	{
	}

	void OpenGLRenderer::Submit(RenderCommand& cmd)
	{
		std::cout << "Submit\n";

		if (cmd.CastsShadow)
		{
			s_ShadowQueue.push_back(cmd);
		}
		if (cmd.material->Opaque)
		{
			s_OpaqueQueue.push_back(cmd);
		}
		else
		{
			s_TransparentQueue.push_back(cmd);
		}

	}
	
	void OpenGLRenderer::Render()
	{
		std::cout << "Render - Shadows\n";
		for (const RenderCommand& cmd : s_ShadowQueue)
		{
		}

		std::cout << "Render - opaque lit\n";
		for (const RenderCommand& cmd : s_OpaqueQueue)
		{
		}

		std::cout << "Render - opaque unlit\n";
		for (const RenderCommand& cmd : s_OpaqueUnlitQueue)
		{
		}

		std::cout << "Render - Transparent lit\n";
		for (const RenderCommand& cmd : s_TransparentQueue)
		{
		}

		std::cout << "Render - Transparent unlit\n";
		for (const RenderCommand& cmd : s_TransparentUnlitQueue)
		{
		}

	}

	void OpenGLRenderer::Release()
	{
	}

	void OpenGLRenderer::ClearQueues()
	{
		s_ShadowQueue.clear();

		s_OpaqueQueue.clear();
		s_OpaqueUnlitQueue.clear();
		s_TransparentQueue.clear();
	}
}