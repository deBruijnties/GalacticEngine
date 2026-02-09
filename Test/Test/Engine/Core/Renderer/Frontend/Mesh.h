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
		Mesh(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors);
		
		// Deconstructor
		~Mesh();

		// Helper Functions
		void RecalculateNormals();
		void RecalculateTangents();
		void RecalculateBounds();

		// Drawing functions 
		void Draw(unsigned int instanceCount);
	private:
		// Innit Backend Mesh
		void Innit();

		// Upload Backend Mesh
		void Upload();

	private:
		bool dirty = true;
		std::vector<Vector3> vertices;
		std::vector<uint32_t> indices;
		std::vector<Vector2> uvs;
		std::vector<Vector3> normals;
		std::vector<Vector4> colors;

		std::unique_ptr<IMesh> mesh;
	};
}

