#pragma once
#include "Camera.h"
#include "../Component.h"
#include <Core/Scene/Gameobject.h>
#include <Core/Rendering/Mesh/Mesh.h>
#include <Core/Rendering/Resources/Material.h>

class StructuredBuffer;

class ParticleSystem : public Component
{
protected:
    struct Particle
    {
        Vector3 position;
        Vector3 velocity;
        float  size;
        Vector4 color;
        float  life;
    };

    std::vector<Particle> particles;
    StructuredBuffer* particleInstanceBuffer = nullptr;

public:

    Mesh* mesh = nullptr;
    Material* material = nullptr;

    int baseAmount = 10;
    float baseLifeTime = 10;
    float baseSpeed = 1;
    float baseSize = 1;
    bool randomVelocity = true;
    bool worldSpace = false;
    bool loop = true;
    bool autostart = true;

    void Burst(int count);
    void Burst(int count, Vector3 worldPosition);

    void OnStart() override;

    void OnUpdate() override;

    void OnSubmitRender() override;
private:
    void SpawnParticle(int count);
    void RemoveParticle(size_t index);
    void ResetParticle(Particle& particle);



};

