#pragma once
#include <Core/engine.h>

struct GLFWwindow;

namespace GalacticEngine::Core::Input
{
    class Mouse
    {
    public:
        
        // Current and previous positions
        static Vector2 position;
        static Vector2 lastPosition;
        static Vector2 delta;
        static Vector2 scroll;

        // Buttons
        static bool buttons[8];
        static bool buttonsDown[8];
        static bool buttonsUp[8];

        // Must be called once at startup
        static void Init(GLFWwindow* window);

        // Must be called every frame
        static void Update();

    private:
        // GLFW internal callbacks
        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void ScrollCallback(GLFWwindow* window, double xoff, double yoff);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    };
}