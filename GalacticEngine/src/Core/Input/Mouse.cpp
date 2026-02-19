#include "pch.h"
#include "Mouse.h"

#include <GLFW/glfw3.h>


namespace GalacticEngine::Core::Input
{

    // Static definitions
    Vector2 Mouse::position = Vector2(0, 0);
    Vector2 Mouse::lastPosition = Vector2(0, 0);
    Vector2 Mouse::delta = Vector2(0, 0);
    Vector2 Mouse::scroll = Vector2(0, 0);

    bool Mouse::buttons[8] = { false };
    bool Mouse::buttonsDown[8] = { false };
    bool Mouse::buttonsUp[8] = { false };


    // INIT — set GLFW callbacks
    void Mouse::Init(GLFWwindow* window)
    {
        glfwSetCursorPosCallback(window, CursorPosCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
    }


    void Mouse::Update()
    {
        // Compute delta movement
        delta = position - lastPosition;
        lastPosition = position;

        // Reset scroll for this frame
        scroll = Vector2(0, 0);

        // Reset button up/down
        for (int i = 0; i < 8; i++)
        {
            buttonsDown[i] = false;
            buttonsUp[i] = false;
        }
    }

    void Mouse::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Mouse::position = Vector2((float)xpos, (float)ypos);
    }


    void Mouse::ScrollCallback(GLFWwindow* window, double xoff, double yoff)
    {
        Mouse::scroll.x += (float)xoff;
        Mouse::scroll.y += (float)yoff;
    }

    void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button < 0 || button >= 8) return;

        if (action == GLFW_PRESS)
        {
            if (!buttons[button])
                buttonsDown[button] = true;

            buttons[button] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            buttons[button] = false;
            buttonsUp[button] = true;
        }
    }

}