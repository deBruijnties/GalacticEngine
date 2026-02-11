#pragma once
#include <vector>
#include <memory>
#include <string>
#include <Core/Math/Types.h>

namespace GalacticEngine
{
	class IMesh
	{
	public:
		virtual ~IMesh() = default;

		virtual void Upload() = 0;
		virtual void Upload(
			const std::vector<Vector3>& vertices,
			const std::vector<uint32_t>& indices,
			const std::vector<Vector2>& uvs,
			const std::vector<Vector3>& normals,
			const std::vector<Vector4>& colors,
			const std::vector<Vector3>& tangents,
			bool dynamicBuffers = false
		) = 0;

		virtual void Draw(uint32_t instanceCount) = 0;
		virtual void Release() = 0;

		bool dynamicBuffers;
		bool dirty = true;
		std::vector<Vector3> vertices;
		std::vector<uint32_t> indices;
		std::vector<Vector2> uvs;
		std::vector<Vector3> normals;
		std::vector<Vector4> colors;
		std::vector<Vector3> tangents;

		std::unique_ptr<IMesh> mesh;
	};
}