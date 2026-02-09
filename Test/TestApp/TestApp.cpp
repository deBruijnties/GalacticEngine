#include <iostream>
#include "Core/Renderer/Renderer.h"
#include <Core/Renderer/Frontend/Mesh.h>

using GalacticEngine::Mesh;


int main()
{
    Mesh testMesh = Mesh();

    testMesh.Draw(1);


    while (true);
}
