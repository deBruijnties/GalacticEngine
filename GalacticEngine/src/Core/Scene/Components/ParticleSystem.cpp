#include <pch.h>
#include "ParticleSystem.h"
#include <Core/Rendering/Buffers/StructuredBuffer.h>
#include <Core/engine.h>
#include <Core/Rendering/Resources/Material.h>
#include "transform.h"
#include <Core/Rendering/Renderer.h>
#include <random>
#include <algorithm>


float Rand01()
{
    static std::mt19937 rng{ std::random_device{}() };
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}

Vector3 RandomUnitVector()
{
    float z = Rand01() * 2.0f - 1.0f;
    float a = Rand01() * 2.0f * 3.1415926535f;
    float r = sqrtf(1.0f - z * z);

    return { r * cosf(a), r * sinf(a), z };
}


void ParticleSystem::ResetParticle(Particle& particle)
{
    if (worldSpace)
        particle.position = transform->worldPosition;
    else
        particle.position = Vector3();

    particle.velocity = randomVelocity ? RandomUnitVector() * baseSpeed : Vector3();
    particle.anglualarVelocity = randomAngularVelocity ? RandomUnitVector() * 5 : Vector3();
    particle.life = 0.0f;
    particle.size = baseSize;
}

void ParticleSystem::SpawnParticle(int count)
{
    if (count <= 0) return;

    particles.reserve(particles.size() + count);

    for (int i = 0; i < count; i++)
    {
        particles.emplace_back();
        ResetParticle(particles.back());
    }
}

void ParticleSystem::Burst(int count, Vector3 worldPosition)
{
    if (count <= 0) return;

    particles.reserve(particles.size() + count);

    for (int i = 0; i < count; i++)
    {
        particles.emplace_back();
        ResetParticle(particles.back());
        particles.back().position = worldPosition;
    }
}

void ParticleSystem::Burst(int count)
{
    SpawnParticle(count);
}


void ParticleSystem::OnStart()
{
    constexpr unsigned int INSTANCE_BINDING = 12;
    constexpr size_t INSTANCE_STRIDE = sizeof(Matrix4);

    if (baseAmount > 0)
        SpawnParticle(baseAmount);

    particleInstanceBuffer = new StructuredBuffer(INSTANCE_BINDING, INSTANCE_STRIDE);
    particleInstanceBuffer->Allocate(std::max<size_t>(1, particles.size()));
}


void ParticleSystem::OnUpdate()
{
    // Ensure GPU buffer capacity
    if (particleInstanceBuffer->GetCapacity() < particles.size())
        particleInstanceBuffer->Allocate(std::max<size_t>(1, particles.size()));

    // If no particles just upload empty
    if (particles.empty())
    {
        particleInstanceBuffer->SetActiveCount(0);
        particleInstanceBuffer->Upload();
        return;
    }
    for (size_t i = 0; i < particles.size(); )
    {
        Particle& p = particles[i];

        p.life += Time::deltaTime;

        if (p.life >= baseLifeTime)
        {
            if (loop)
            {
                ResetParticle(p);
                ++i;
                continue;
            }
            else
            {
                particles[i] = particles.back();
                particles.pop_back();
                continue; // do NOT increment i
            }
        }

        // Simulate
        p.position += p.velocity * Time::deltaTime;
        p.rotation += p.anglualarVelocity * Time::deltaTime;

        float t = p.life / baseLifeTime;
        p.size = Math::Lerp(baseSize, 0.0f, t);

        Matrix4 m = Matrix4::Identity();
        m = Math::Translate(m, p.position);
        m = Math::Rotate(m, Quaternion::FromEuler(p.rotation));
        m = Math::Scale(m, Vector3(p.size, p.size, p.size));

        if (!worldSpace)
            m = transform->worldMatrix * m;

        particleInstanceBuffer->Set(i, m);

        ++i;
    }


    // Sync GPU count
    const size_t aliveCount = particles.size();
    particleInstanceBuffer->SetActiveCount(aliveCount);

    // Debug safety
    if (aliveCount > particleInstanceBuffer->GetCapacity())
    {
        std::cout << "ERROR: aliveCount > capacity\n";
        particleInstanceBuffer->SetActiveCount(particleInstanceBuffer->GetCapacity());
    }

    // Upload
    particleInstanceBuffer->Upload();
}


void ParticleSystem::OnSubmitRender()
{
    if (!material || !mesh || !particleInstanceBuffer)
        return;

    if (particleInstanceBuffer->GetActiveCount() == 0)
        return;

    Renderer::InstancedRenderCommand cmd;
    cmd.mesh = mesh;
    cmd.material = material;
    cmd.CastsShadow = true;
    cmd.instanceBuffer = particleInstanceBuffer;

    Renderer::SubmitInstanced(cmd);
}
