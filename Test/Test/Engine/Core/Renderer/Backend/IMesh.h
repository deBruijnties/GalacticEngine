#pragma once
#include <vector>
#include <string>
#include <Core/Math/Types.h>

namespace GalacticEngine
{
	class IMesh
	{
	public:
		virtual ~IMesh() = default;

		virtual void Upload(
			const std::vector<Vector3>& vertices,
			const std::vector<uint32_t>& indices,
			const std::vector<Vector2>& uvs,
			const std::vector<Vector3>& normals,
			const std::vector<Vector4>& colors
		) = 0;

		virtual void Draw(uint32_t instanceCount) = 0;

		virtual void Release() = 0;
	};
}