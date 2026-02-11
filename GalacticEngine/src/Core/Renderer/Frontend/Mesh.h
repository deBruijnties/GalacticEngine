#pragma once
#include <vector>
#include <string>
#include <memory>
#include <Core/Math/Types.h>
#include <Core/Renderer/Backend/IMesh.h>

namespace GalacticEngine
{
	class Mesh
	{
	public:
		// Constructors
		Mesh();
		Mesh(const std::string Path);
		Mesh(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors, const std::vector<Vector4>& tangents, bool dynamicBuffers);
		
		// Deconstructor
		~Mesh();

		// Helper Functions
		void RecalculateNormals();
		void RecalculateTangents();
		void RecalculateBounds();

		// Drawing functions 
		void Draw(unsigned int instanceCount);

		IMesh* GetBackend();

		// Innit Backend Mesh
		void Init();

		// Upload Backend Mesh
		void Upload();

		std::unique_ptr<IMesh> mesh;

	};
}

