#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

// these macros do nothing in release builds so there's zero overhead in shipping code
// in debug you just drop PROFILE_SCOPE("whatever") at the top of any scope and it tracks it
#if _DEBUG
#define PROFILE_SCOPE(name) Profiler::ScopedTimer timer##__LINE__(name)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCTION__) // uses the actual function name automatically
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

namespace GalacticEngine::Core::Profiling
{
    class Profiler
    {
    public:

        // per-scope accumulator - tracks total time spent and how many times it was hit this frame
        struct Sample
        {
            double   totalMs = 0.0;
            uint32_t calls = 0;
        };

        // RAII timer - starts the clock on construction, records the result on destruction
        // works for any scope: functions, loops, if-blocks, whatever you need
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
                Profiler::Get().AddSample(m_Name, ms); // push result into this frame's data
            }

        private:
            const char* m_Name;
            std::chrono::high_resolution_clock::time_point m_Start;
        };

    public:

        // singleton - one profiler for the whole engine, no need to pass it around
        static Profiler& Get()
        {
            static Profiler instance;
            return instance;
        }

        // call at the very start of the frame (before any game logic runs)
        void BeginFrame()
        {
            m_FrameSamples.clear(); // wipe last frame's data, we only care about the current frame
            m_FrameStart = Clock::now();
        }

        // call at the very end of the frame (after rendering, before swap)
        void EndFrame()
        {
            auto now = Clock::now();
            m_FrameTimeMs =
                std::chrono::duration<double, std::milli>(now - m_FrameStart).count();

            // FPS is averaged over a full second so it doesn't flicker like crazy
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

        // called internally by ScopedTimer's destructor - don't call this manually
        void AddSample(const std::string& name, double ms)
        {
            auto& s = m_FrameSamples[name];
            s.totalMs += ms; // accumulate in case the same scope gets hit multiple times per frame
            s.calls++;
        }

        // spits everything out to stdout - handy for a quick sanity check, not meant for live overlay
        void DumpToConsole()
        {
            std::cout << "\n- PROFILER FRAME -\n";
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

        // getters for anything that wants to display FPS/frametime in a UI overlay
        int    GetFPS()         const { return m_FPS; }
        double GetFrameTimeMs() const { return m_FrameTimeMs; }

    private:
        using Clock = std::chrono::high_resolution_clock;

        std::unordered_map<std::string, Sample> m_FrameSamples; // reset every frame
        Clock::time_point                        m_FrameStart;
        double                                   m_FrameTimeMs = 0.0;

        // FPS smoothing state - keeps a rolling 1-second window
        double   m_AccumulatedTime = 0.0;
        uint32_t m_FrameCounter = 0;
        int      m_FPS = 0;
    };
}