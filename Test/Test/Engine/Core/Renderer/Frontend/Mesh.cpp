#include "Mesh.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/APIs/OpenGL/OpenGLMesh.h"
#include <iostream>

#define ASSIMP_BUILD_STATIC_LIB
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GalacticEngine::Mesh::Mesh()
{
	//Create Backend
	Init();
	//set vectors
	
	// upload to GPU
	Upload();
}

GalacticEngine::Mesh::Mesh(const std::string Path)
{
	//Create Backend
	Init();
	
	//set vectors
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(Path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Failed to load model: " << Path << "\n";
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    const aiMesh* aiMesh = scene->mMeshes[0]; // for simplicity, just load the first mesh
    
    mesh->vertices.reserve(aiMesh->mNumVertices);
    mesh->normals.reserve(aiMesh->mNumVertices);
    mesh->uvs.reserve(aiMesh->mNumVertices);
    mesh->colors.reserve(aiMesh->mNumVertices);
    mesh->tangents.reserve(aiMesh->mNumVertices);

    // Extract vertex data
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        mesh->vertices.emplace_back(
            aiMesh->mVertices[i].x,
            aiMesh->mVertices[i].y,
            aiMesh->mVertices[i].z
        );

        if (aiMesh->HasNormals()) {
            mesh->normals.emplace_back(
                aiMesh->mNormals[i].x,
                aiMesh->mNormals[i].y,
                aiMesh->mNormals[i].z
            );
        }

        if (aiMesh->HasTextureCoords(0)) {
            mesh->uvs.emplace_back(
                aiMesh->mTextureCoords[0][i].x,
                1.0f - aiMesh->mTextureCoords[0][i].y
            );
        }

        if (aiMesh->HasVertexColors(0)) {
            const aiColor4D& c = aiMesh->mColors[0][i];
            mesh->colors.emplace_back(c.r, c.g, c.b, c.a);
        }

        if (aiMesh->HasTangentsAndBitangents())
        {
            mesh->tangents.emplace_back(
                aiMesh->mTangents[i].x,
                aiMesh->mTangents[i].y,
                aiMesh->mTangents[i].z
            );
        }
    }

    // Extract indices
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        const aiFace& face = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            mesh->indices.push_back(face.mIndices[j]);
        }
    }

	// upload to GPU
	Upload();
}

GalacticEngine::Mesh::Mesh(const std::vector<Vector3>& _vertices,
    const std::vector<unsigned int>& _indices,
    const std::vector<Vector2>& _uvs,
    const std::vector<Vector3>& _normals,
    const std::vector<Vector4>& _colors,
    const std::vector<Vector4>& _tangents,
    bool dynamicBuffers)
{
	//Create Backend
	Init();
	
	// set vectors
	mesh->vertices = _vertices;
	mesh->indices = _indices;
	mesh->uvs = _uvs;
	mesh->normals = _normals;
	mesh->colors = _colors;

	// upload to GPU
	Upload();
}

GalacticEngine::Mesh::~Mesh()
{
	mesh->Release();
}

void GalacticEngine::Mesh::RecalculateNormals()
{
	std::cout << "RecalculateNormals\n";
}

void GalacticEngine::Mesh::RecalculateTangents()
{
	std::cout << "RecalculateTangents\n";
}

void GalacticEngine::Mesh::RecalculateBounds()
{
	std::cout << "RecalculateBounds\n";
}

void GalacticEngine::Mesh::Draw(unsigned int instanceCount)
{
	if (mesh->dirty)
		Upload();
	mesh->Draw(instanceCount);
}

void GalacticEngine::Mesh::Init()
{
	switch (Renderer::GetGraphicsApi())
	{
	case GraphicsApi::OpenGL_es:
	case GraphicsApi::OpenGL:
		mesh = std::unique_ptr<IMesh>(new OpenGLMesh());
		break;
	case GraphicsApi::Vulkan:
		//mesh = std::unique_ptr<IMesh>(new VulkanMesh());
        std::cout << "Graphics API not Supported YET\n";
        break;
	default:
		std::cout << "Graphics API not Supported\n";
	}
}

void GalacticEngine::Mesh::Upload()
{
	mesh->Upload();
}

GalacticEngine::IMesh* GalacticEngine::Mesh::GetBackend()
{
    return mesh.get();
}


