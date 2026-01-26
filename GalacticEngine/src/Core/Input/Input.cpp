#include "pch.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <cstring>

namespace GalacticEngine::Core::Input
{


    static bool s_CurrentKeys[(int)Key::Count];
    static bool s_PreviousKeys[(int)Key::Count];

    static Key::Code TranslateGLFWKey(int glfwKey)
    {
        switch (glfwKey)
        {
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_B: return Key::B;
        case GLFW_KEY_C: return Key::C;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_E: return Key::E;
        case GLFW_KEY_F: return Key::F;
        case GLFW_KEY_G: return Key::G;
        case GLFW_KEY_H: return Key::H;
        case GLFW_KEY_I: return Key::I;
        case GLFW_KEY_J: return Key::J;
        case GLFW_KEY_K: return Key::K;
        case GLFW_KEY_L: return Key::L;
        case GLFW_KEY_M: return Key::M;
        case GLFW_KEY_N: return Key::N;
        case GLFW_KEY_O: return Key::O;
        case GLFW_KEY_P: return Key::P;
        case GLFW_KEY_Q: return Key::Q;
        case GLFW_KEY_R: return Key::R;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_T: return Key::T;
        case GLFW_KEY_U: return Key::U;
        case GLFW_KEY_V: return Key::V;
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_X: return Key::X;
        case GLFW_KEY_Y: return Key::Y;
        case GLFW_KEY_Z: return Key::Z;

        case GLFW_KEY_0: return Key::Num0;
        case GLFW_KEY_1: return Key::Num1;
        case GLFW_KEY_2: return Key::Num2;
        case GLFW_KEY_3: return Key::Num3;
        case GLFW_KEY_4: return Key::Num4;
        case GLFW_KEY_5: return Key::Num5;
        case GLFW_KEY_6: return Key::Num6;
        case GLFW_KEY_7: return Key::Num7;
        case GLFW_KEY_8: return Key::Num8;
        case GLFW_KEY_9: return Key::Num9;

        case GLFW_KEY_SPACE: return Key::Space;
        case GLFW_KEY_ESCAPE: return Key::Escape;
        case GLFW_KEY_ENTER: return Key::Enter;
        case GLFW_KEY_TAB: return Key::Tab;
        case GLFW_KEY_BACKSPACE: return Key::Backspace;

        case GLFW_KEY_LEFT: return Key::Left;
        case GLFW_KEY_RIGHT: return Key::Right;
        case GLFW_KEY_UP: return Key::Up;
        case GLFW_KEY_DOWN: return Key::Down;

        case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
        case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
        case GLFW_KEY_LEFT_CONTROL: return Key::LeftCtrl;
        case GLFW_KEY_RIGHT_CONTROL: return Key::RightCtrl;
        case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
        case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;

        case GLFW_KEY_F1: return Key::F1;
        case GLFW_KEY_F2: return Key::F2;
        case GLFW_KEY_F3: return Key::F3;
        case GLFW_KEY_F4: return Key::F4;
        case GLFW_KEY_F5: return Key::F5;
        case GLFW_KEY_F6: return Key::F6;
        case GLFW_KEY_F7: return Key::F7;
        case GLFW_KEY_F8: return Key::F8;
        case GLFW_KEY_F9: return Key::F9;
        case GLFW_KEY_F10: return Key::F10;
        case GLFW_KEY_F11: return Key::F11;
        case GLFW_KEY_F12: return Key::F12;
        }

        return Key::Unknown;
    }

    void Input::Init(void* nativeWindowHandle)
    {
        GLFWwindow* window = (GLFWwindow*)nativeWindowHandle;

        std::memset(s_CurrentKeys, 0, sizeof(s_CurrentKeys));
        std::memset(s_PreviousKeys, 0, sizeof(s_PreviousKeys));

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods)
            {
                Key::Code k = TranslateGLFWKey(key);
                if (k == Key::Unknown) return;

                if (action == GLFW_PRESS)
                    s_CurrentKeys[(int)k] = true;
                else if (action == GLFW_RELEASE)
                    s_CurrentKeys[(int)k] = false;
            });
    }

    void Input::Update()
    {
        std::memcpy(s_PreviousKeys, s_CurrentKeys, sizeof(s_CurrentKeys));
    }

    bool Input::IsKeyDown(Key::Code key)
    {
        return s_CurrentKeys[(int)key];
    }

    bool Input::IsKeyPressed(Key::Code key)
    {
        return s_CurrentKeys[(int)key] && !s_PreviousKeys[(int)key];
    }

    bool Input::IsKeyReleased(Key::Code key)
    {
        return !s_CurrentKeys[(int)key] && s_PreviousKeys[(int)key];
    }

}