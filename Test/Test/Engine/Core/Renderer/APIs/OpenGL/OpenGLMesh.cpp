#include "OpenGLMesh.h"
#include <iostream>
#include <GLAD/glad.h>

void GalacticEngine::OpenGLMesh::Draw(uint32_t instanceCount)
{
    if (dirty)
        Upload();
    if (instanceCount > 1)
    {

    }
    else
    {
        if (_vao == 0) {
            throw std::runtime_error("Mesh buffers have not been generated. Call GenerateBuffers() before drawing.");
        }

        // Ensure the shader is in use

        // Bind the VAO for this mesh
        glBindVertexArray(_vao);



        // Draw the mesh
        if (!indices.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        }

        glBindVertexArray(0); // Unbind VAO
    }
   
}

void GalacticEngine::OpenGLMesh::Upload()
{
    dirty = false;
    GLenum usage = dynamicBuffers ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Generate and bind VAO
    if (_vao == 0)
        glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // --- Vertex Buffer (positions) ---
    if (_vbo == 0)
        glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), usage);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Tangent Buffer ---
    if (!tangents.empty())
    {
        if (_tbo == 0)
            glGenBuffers(1, &_tbo);

        glBindBuffer(GL_ARRAY_BUFFER, _tbo);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vector3), tangents.data(), usage);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(4);
    }

    // --- Normal Buffer ---
    if (!normals.empty()) {
        if (_nbo == 0)
            glGenBuffers(1, &_nbo);
        glBindBuffer(GL_ARRAY_BUFFER, _nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), usage);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // --- Color Buffer ---
    if (!colors.empty()) {
        if (_cbo == 0)
            glGenBuffers(1, &_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, _cbo);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector4), colors.data(), usage);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // --- UV Buffer ---
    if (!uvs.empty()) {
        if (_uvbo == 0)
            glGenBuffers(1, &_uvbo);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), uvs.data(), usage);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
        glEnableVertexAttribArray(3);
    }

    // --- Element Buffer (EBO) ---
    if (!indices.empty()) {
        if (_ebo == 0)
            glGenBuffers(1, &_ebo);

        // Diagnostic: print pointer & size before upload
        std::cout << "[EBO-DIAG] Indices.size() = " << indices.size()
            << ", Indices.data() = " << (void*)indices.data() << '\n';

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
            static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
            indices.data(),
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
        for (auto i : indices) if (i > maxIdx) maxIdx = i;
        std::cout << "[VERIFY] max index = " << maxIdx << ", vertex count = " << vertices.size() << '\n';

        // Unbind VAO (leave EBO attached to VAO)
        glBindVertexArray(0);
    }
    std::cout << "[Mesh] Vertices: " << vertices.size() << "\n";
}

void GalacticEngine::OpenGLMesh::Upload(const std::vector<Vector3>& _vertices,
    const std::vector<uint32_t>& _indices,
    const std::vector<Vector2>& _uvs,
    const std::vector<Vector3>& _normals,
    const std::vector<Vector4>& _colors,
    const std::vector<Vector3>& _tangents,
    bool _dynamicBuffers)
{
    vertices = _vertices;
    indices = _indices;
    uvs = _uvs;
    normals = _normals;
    colors = _colors;
    tangents = _tangents;

    GLenum usage = dynamicBuffers ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Generate and bind VAO
    if (_vao == 0)
        glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // --- Vertex Buffer (positions) ---
    if (_vbo == 0)
        glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), usage);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Tangent Buffer ---
    if (!tangents.empty())
    {
        if (_tbo == 0)
            glGenBuffers(1, &_tbo);

        glBindBuffer(GL_ARRAY_BUFFER, _tbo);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vector3), tangents.data(), usage);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(4);
    }

    // --- Normal Buffer ---
    if (!normals.empty()) {
        if (_nbo == 0)
            glGenBuffers(1, &_nbo);
        glBindBuffer(GL_ARRAY_BUFFER, _nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), usage);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // --- Color Buffer ---
    if (!colors.empty()) {
        if (_cbo == 0)
            glGenBuffers(1, &_cbo);
        glBindBuffer(GL_ARRAY_BUFFER, _cbo);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector4), colors.data(), usage);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // --- UV Buffer ---
    if (!uvs.empty()) {
        if (_uvbo == 0)
            glGenBuffers(1, &_uvbo);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), uvs.data(), usage);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
        glEnableVertexAttribArray(3);
    }

    // --- Element Buffer (EBO) ---
    if (!indices.empty()) {
        if (_ebo == 0)
            glGenBuffers(1, &_ebo);

        // Diagnostic: print pointer & size before upload
        std::cout << "[EBO-DIAG] Indices.size() = " << indices.size()
            << ", Indices.data() = " << (void*)indices.data() << '\n';

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
            static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
            indices.data(),
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
        for (auto i : indices) if (i > maxIdx) maxIdx = i;
        std::cout << "[VERIFY] max index = " << maxIdx << ", vertex count = " << vertices.size() << '\n';

        // Unbind VAO (leave EBO attached to VAO)
        glBindVertexArray(0);
    }
    std::cout << "[Mesh] Vertices: " << vertices.size() << "\n";
}

void GalacticEngine::OpenGLMesh::Release()
{
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
    if (_tbo != 0)
        glDeleteBuffers(1, &_tbo);
}
void GalacticEngine::OpenGLMesh::Bind()
{
    glBindVertexArray(_vao);
}

void GalacticEngine::OpenGLMesh::Unbind()
{
    glBindVertexArray(0);
}
