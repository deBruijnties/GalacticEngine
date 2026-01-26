#pragma once

#include "KeyCodes.h"

struct GLFWwindow; // forward declare, no glfw include

namespace GalacticEngine::Core::Input
{

    class Input
    {
    public:
        static void Init(void* nativeWindowHandle); // void* so no GLFW types leak
        static void Update();

        static bool IsKeyDown(Key::Code key);
        static bool IsKeyPressed(Key::Code key);
        static bool IsKeyReleased(Key::Code key);

    private:
        static void KeyCallback(void* window, int key, int scancode, int action, int mods);
    };

}