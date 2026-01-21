#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

namespace GalacticEngine::Core::Debug
{

#if _DEBUG
#define PROFILE_SCOPE(name) Profiler::ScopedTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

    class Profiler
    {
    public:
        struct Sample
        {
            double totalMs = 0.0;
            uint32_t calls = 0;
        };

        class ScopedTimer
        {
        public:
            ScopedTimer(const char* name)
                : m_Name(name),
                m_Start(std::chrono::high_resolution_clock::now())
            {
            }

            ~ScopedTimer()
            {
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - m_Start).count();
                Profiler::Get().AddSample(m_Name, ms);
            }

        private:
            const char* m_Name;
            std::chrono::high_resolution_clock::time_point m_Start;
        };

    public:
        static Profiler& Get()
        {
            static Profiler instance;
            return instance;
        }

        // Call once per frame
        void BeginFrame()
        {
            m_FrameSamples.clear();
            m_FrameStart = Clock::now();
        }

        // Call once per frame
        void EndFrame()
        {
            auto now = Clock::now();
            m_FrameTimeMs =
                std::chrono::duration<double, std::milli>(now - m_FrameStart).count();

            // FPS smoothing
            m_AccumulatedTime += m_FrameTimeMs;
            m_FrameCounter++;

            if (m_AccumulatedTime >= 1000.0)
            {
                m_FPS = static_cast<int>(
                    (m_FrameCounter * 1000.0) / m_AccumulatedTime
                    );
                m_AccumulatedTime = 0.0;
                m_FrameCounter = 0;
            }
        }

        void AddSample(const std::string& name, double ms)
        {
            auto& s = m_FrameSamples[name];
            s.totalMs += ms;
            s.calls++;
        }

        void DumpToConsole()
        {
            std::cout << "\n---- PROFILER FRAME ----\n";
            std::cout << "FPS: " << m_FPS << "\n";
            std::cout << "Frame time: " << m_FrameTimeMs << " ms\n\n";

            for (auto& [name, s] : m_FrameSamples)
            {
                double percent = (s.totalMs / m_FrameTimeMs) * 100.0;

                std::cout
                    << std::left << std::setw(25) << name
                    << " | " << std::setw(6) << s.totalMs << " ms"
                    << " | " << std::setw(6) << percent << " %"
                    << " | calls: " << s.calls
                    << "\n";
            }
        }

        int GetFPS() const { return m_FPS; }
        double GetFrameTimeMs() const { return m_FrameTimeMs; }

    private:
        using Clock = std::chrono::high_resolution_clock;

        std::unordered_map<std::string, Sample> m_FrameSamples;

        Clock::time_point m_FrameStart;
        double m_FrameTimeMs = 0.0;

        // FPS tracking
        double m_AccumulatedTime = 0.0;
        uint32_t m_FrameCounter = 0;
        int m_FPS = 0;
    };
}