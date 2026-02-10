#pragma once
#include <memory>
#include <vector>
#include <Core/engine.h>
#include <string>

class Mesh {
public:

    Mesh();
    Mesh(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices, const std::vector<Vector2>& uvs, const std::vector<Vector3>& normals, const std::vector<Vector4>& colors);
    ~Mesh();
    static Mesh LoadMeshFromFile(const std::string& filepath);


    void Bind() const;
    void Unbind() const;
    void GenerateBuffers(bool dynamicDraw = false);
    void GenerateNormals();
    void Draw() const;
    void DrawInstanced(unsigned int instanceCount) const;

private:
    unsigned int _vao = 0;
    unsigned int _vbo = 0;
    unsigned int _ebo = 0;
    unsigned int _nbo = 0;
    unsigned int _cbo = 0;
    unsigned int _uvbo = 0;
    unsigned int _tbo = 0;

protected:
    std::vector<Vector3> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Vector3> Normals;
    std::vector<Vector4> Colors;
    std::vector<Vector2> UVs;
    std::vector<Vector3> Tangents;

};

