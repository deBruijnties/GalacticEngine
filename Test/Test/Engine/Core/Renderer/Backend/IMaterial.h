#pragma once
#include <string>
#include "core/Renderer/Frontend/Shader.h"
#include "core/Math/Types.h"

namespace GalacticEngine
{
	class IMaterial
	{
	public:
		


		virtual void Release() = 0;
	private:
		Shader* shader = nullptr;
	};

}