#include "pch.h"
#include "Time.h"

namespace GalacticEngine::Core
{
    // --- Define static variables ---
    float Time::deltaTime = 0.0f;
    float Time::timeSinceStartup = 0.0f;
    float Time::timeScale = 1.0f;
    float Time::fps = 0;

    void Time::Initialize()
    {
        timeSinceStartup = 0.0f;
        deltaTime = 0.0f;
        timeScale = 1.0f;
    }

    void Time::Update(float dt)
    {
        deltaTime = dt * timeScale;
        timeSinceStartup += deltaTime;
        fps = 1.0f / dt;
    }
}