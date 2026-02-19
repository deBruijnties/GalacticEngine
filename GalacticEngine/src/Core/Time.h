#pragma once

namespace GalacticEngine::Core
{
    class Time
    {
    public:
        static float deltaTime;             // Time between frames
        static float timeSinceStartup;      // Accumulated time since Init()
        static float timeScale;             // Multiply dt by this (1.0 = normal)
        static float fps;                   // Frames per second

        static void Initialize();

        static void Update(float dt);

        static float GetTime() { return timeSinceStartup; }
    };
}
