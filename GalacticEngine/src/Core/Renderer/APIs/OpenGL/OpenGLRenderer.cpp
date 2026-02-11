#include "OpenGLRenderer.h"
#include <core/Renderer/APIs/OpenGL/OpenGLMaterial.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

namespace GalacticEngine
{
	std::vector<RenderCommand> OpenGLRenderer::s_OpaqueQueue;
	std::vector<RenderCommand> OpenGLRenderer::s_OpaqueUnlitQueue;

	// Transparent Queues
	std::vector<RenderCommand> OpenGLRenderer::s_TransparentQueue;
	std::vector<RenderCommand> OpenGLRenderer::s_TransparentUnlitQueue;

	// Shadow Queues
	std::vector<RenderCommand> OpenGLRenderer::s_ShadowQueue;



	OpenGLRenderer::OpenGLRenderer()
	{
		std::cout << "initializing GLAD\n";
		// Load OpenGL functions
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD\n";
			glfwTerminate();
			return;
		}
	}

	void OpenGLRenderer::Submit(RenderCommand& cmd)
	{

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
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//std::cout << "Render - Shadows\n";
		for (const RenderCommand& cmd : s_ShadowQueue)
		{
			OpenGLMaterial* glMat = static_cast<OpenGLMaterial*>(cmd.material->GetBackend()); // raw pointer cast
			glMat->Use();
			cmd.mesh->Draw(1);
		}

		//std::cout << "Render - opaque lit\n";
		for (const RenderCommand& cmd : s_OpaqueQueue)
		{
		}

		//std::cout << "Render - opaque unlit\n";
		for (const RenderCommand& cmd : s_OpaqueUnlitQueue)
		{
		}

		//std::cout << "Render - Transparent lit\n";
		for (const RenderCommand& cmd : s_TransparentQueue)
		{
		}

		//std::cout << "Render - Transparent unlit\n";
		for (const RenderCommand& cmd : s_TransparentUnlitQueue)
		{
		}

		
		ClearQueues();
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