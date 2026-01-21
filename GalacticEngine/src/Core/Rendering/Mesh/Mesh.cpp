#include "pch.h"
#include "Mesh.h"
#include <stdexcept>
#include <glad/glad.h>
#include <iostream>


#define ASSIMP_BUILD_STATIC_LIB
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Mesh::Mesh()
    : _vao(0), _vbo(0), _ebo(0), _nbo(0), _cbo(0), _uvbo(0)
{
    // You can still access inherited members here if needed:
    Vertices.clear();
    Normals.clear();
    Colors.clear();
    UVs.clear();
    Indices.clear();
}


Mesh::Mesh(
    const std::vector<Vector3>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<Vector2>& uvs,
    const std::vector<Vector3>& normals,
    const std::vector<Vector4>& colors)
    : _vao(0), _vbo(0), _ebo(0), _nbo(0), _cbo(0), _uvbo(0)
{
    Vertices = vertices;
    Indices = indices;
    UVs = uvs;
    Normals = normals;
    Colors = colors;
}

Mesh::~Mesh() {
    // Delete OpenGL buffers if they have been created
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
    }
    if (_ebo != 0) {
        glDeleteBuffers(1, &_ebo);
    }
    if (_nbo != 0) {
        glDeleteBuffers(1, &_nbo);
    }
    if (_cbo != 0) {
        glDeleteBuffers(1, &_cbo);
    }
    if (_uvbo != 0) {
        glDeleteBuffers(1, &_uvbo);
    }
    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
    }
}

void Mesh::Bind() const
{
    glBindVertexArray(_vao);
}

void Mesh::Unbind() const
{
    glBindVertexArray(0);
}

void Mesh::GenerateBuffers(bool dynamicDraw)
{
    GLenum usage = dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Generate and bind VAO
    if (_vao == 0)
        glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // --- Vertex Buffer (positions) ---
    if (_vbo == 0)
        glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vector3), Vertices.data(), usage);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Normal Buffer ---
    if (!Normals.empty()) {
        if (_nbo == 0)
            glGenBuffers(1, &_nbo);
        glBindBuffer(GL_ARRAY_BUFFER, _nbo);
        glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Vector3), Normals.data(), usage);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // --- Color Buffer ---
    if (!Colors.empty()) {
        if (_cbo == 0)
            glGenBuffers(1, &_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, _cbo);
        glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Vector4), Colors.data(), usage);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // --- UV Buffer ---
    if (!UVs.empty()) {
        if (_uvbo == 0)
            glGenBuffers(1, &_uvbo);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbo);
        glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(Vector2), UVs.data(), usage);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
        glEnableVertexAttribArray(3);
    }

    // --- Element Buffer (EBO) ---
    if (!Indices.empty()) {
        if (_ebo == 0)
            glGenBuffers(1, &_ebo);

        // Diagnostic: print pointer & size before upload
        std::cout << "[EBO-DIAG] Indices.size() = " << Indices.size()
            << ", Indices.data() = " << (void*)Indices.data() << '\n';

        // Clear GL error, then bind & upload
        while (glGetError() != GL_NO_ERROR) {} // clear previous errors

        // Ensure VAO is bound while uploading EBO
        glBindVertexArray(_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

        GLenum errBefore = glGetError();
        if (errBefore != GL_NO_ERROR) {
            std::cout << "[EBO-DIAG] Error BEFORE glBufferData: 0x" << std::hex << errBefore << std::dec << '\n';
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(Indices.size() * sizeof(unsigned int)),
            Indices.data(),
            usage);

        GLenum errAfter = glGetError();
        if (errAfter != GL_NO_ERROR) {
            std::cout << "[EBO-DIAG] Error AFTER glBufferData: 0x" << std::hex << errAfter << std::dec << '\n';
        }

        // Verify buffer size on GPU
        GLint checkSize = 0;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &checkSize);
        std::cout << "[VERIFY] EBO size after upload = " << checkSize << '\n';

        // Check max index sanity
        unsigned int maxIdx = 0;
        for (auto i : Indices) if (i > maxIdx) maxIdx = i;
        std::cout << "[VERIFY] max index = " << maxIdx << ", vertex count = " << Vertices.size() << '\n';

        // Unbind VAO (leave EBO attached to VAO)
        glBindVertexArray(0);
    }


}



Mesh Mesh::LoadMeshFromFile(const std::string& filepath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Failed to load model: " << filepath << "\n";
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return Mesh();
    }

    const aiMesh* aiMesh = scene->mMeshes[0]; // for simplicity, just load the first mesh

    Mesh mesh;
    mesh.Vertices.reserve(aiMesh->mNumVertices);
    mesh.Normals.reserve(aiMesh->mNumVertices);
    mesh.UVs.reserve(aiMesh->mNumVertices);
    mesh.Colors.reserve(aiMesh->mNumVertices);

    // --- Extract vertex data ---
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        mesh.Vertices.emplace_back(
            aiMesh->mVertices[i].x,
            aiMesh->mVertices[i].y,
            aiMesh->mVertices[i].z
        );

        if (aiMesh->HasNormals()) {
            mesh.Normals.emplace_back(
                aiMesh->mNormals[i].x,
                aiMesh->mNormals[i].y,
                aiMesh->mNormals[i].z
            );
        }

        if (aiMesh->HasTextureCoords(0)) {
            mesh.UVs.emplace_back(
                aiMesh->mTextureCoords[0][i].x,
                1.0f - aiMesh->mTextureCoords[0][i].y
            );
        }

        if (aiMesh->HasVertexColors(0)) {
            const aiColor4D& c = aiMesh->mColors[0][i];
            mesh.Colors.emplace_back(c.r, c.g, c.b, c.a);
        }
    }

    // --- Extract indices ---
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        const aiFace& face = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            mesh.Indices.push_back(face.mIndices[j]);
        }
    }

    // --- Upload to GPU ---

    // --- Calculate buffer sizes ---
    size_t vertexBufferSize = mesh.Vertices.size() * sizeof(Vector3);
    size_t normalBufferSize = mesh.Normals.size() * sizeof(Vector3);
    size_t uvBufferSize = mesh.UVs.size() * sizeof(Vector2);
    size_t colorBufferSize = mesh.Colors.size() * sizeof(Vector4);
    size_t indexBufferSize = mesh.Indices.size() * sizeof(unsigned int);

    size_t totalBufferSize = vertexBufferSize + normalBufferSize + uvBufferSize + colorBufferSize + indexBufferSize;

   

    return mesh;
}
void Mesh::GenerateNormals()
{
    Normals.clear();
    Normals.resize(Vertices.size(), Vector3());

    // If indexed mesh
    if (!Indices.empty())
    {
        for (size_t i = 0; i < Indices.size(); i += 3)
        {
            unsigned int i0 = Indices[i + 0];
            unsigned int i1 = Indices[i + 1];
            unsigned int i2 = Indices[i + 2];

            const Vector3& v0 = Vertices[i0];
            const Vector3& v1 = Vertices[i1];
            const Vector3& v2 = Vertices[i2];

            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;
            
            Vector3 normal = Math::Normalize(Math::Cross(edge1, edge2));

            Normals[i0] += normal;
            Normals[i1] += normal;
            Normals[i2] += normal;
        }

        // Normalize all
        for (auto& n : Normals)
            n = Math::Normalize(n);
    }
    else
    {
        // Non-indexed mesh (triangle soup)
        for (size_t i = 0; i < Vertices.size(); i += 3)
        {
            const Vector3& v0 = Vertices[i + 0];
            const Vector3& v1 = Vertices[i + 1];
            const Vector3& v2 = Vertices[i + 2];

            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;

            Vector3 normal = Math::Normalize(Math::Cross(edge1, edge2));

            Normals[i + 0] = normal;
            Normals[i + 1] = normal;
            Normals[i + 2] = normal;
        }
    }

    std::cout << "[Mesh] Generated normals for " << Vertices.size() << " vertices.\n";
}

void Mesh::Draw() const
{
    if (_vao == 0) {
        throw std::runtime_error("Mesh buffers have not been generated. Call GenerateBuffers() before drawing.");
    }

    // Ensure the shader is in use

    // Bind the VAO for this mesh
    glBindVertexArray(_vao);



    // Draw the mesh
    if (!Indices.empty()) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Vertices.size()));
    }

    glBindVertexArray(0); // Unbind VAO
}

void Mesh::DrawInstanced(unsigned int instanceCount) const
{
    glBindVertexArray(_vao);

    if (!Indices.empty())
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
    else
        glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)Vertices.size(), instanceCount);

    glBindVertexArray(0);
}



