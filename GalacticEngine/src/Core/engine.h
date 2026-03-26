#pragma once
#include <Core/Math/Math.h>
#include <Core/Time.h>
#include <Core/Debug/Profiler.h>
#include <Core/Input/Mouse.h>
#include <Core/Input/Input.h>

struct GLFWwindow;
class Scene;

namespace GalacticEngine::Core
{
    class Engine {
    public:
        Engine(int width = 800, int height = 600, const char* title = "GalacticEngineWindow");
        virtual ~Engine();

        void Run();

        static void SetTitle(const char* title);

        static Scene* getCurrentScene();
        static void setCurrentScene(Scene* scene);

        static bool Running;
        static int width;
        static int height;

    protected:
        static GLFWwindow* window;
        static Scene* currentScene;

        void Resize(int width, int height);
        void Render();

        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnProcessInput() {}

        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    };

}

#pragma once

namespace Math = GalacticEngine::Math;
namespace Input = GalacticEngine::Core::Input;

using Profiler = GalacticEngine::Core::Profiling::Profiler;

using Vector2 = GalacticEngine::Vector2;
using Vector3 = GalacticEngine::Vector3;
using Vector4 = GalacticEngine::Vector4;
using Vector2Int = GalacticEngine::Vector2Int;
using Vector3Int = GalacticEngine::Vector3Int;
using Vector4Int = GalacticEngine::Vector4Int;
using Matrix4 = GalacticEngine::Matrix4;
using Quaternion = GalacticEngine::Quaternion;
using Engine = GalacticEngine::Core::Engine;

using Time = GalacticEngine::Core::Time;

