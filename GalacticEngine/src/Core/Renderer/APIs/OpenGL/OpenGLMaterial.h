#pragma once
#include "core/Renderer/Backend/IMaterial.h"
namespace GalacticEngine
{
	class OpenGLMaterial : public IMaterial
	{
	public:
		void Use();

		void Release() override;


	private:

	};
}



