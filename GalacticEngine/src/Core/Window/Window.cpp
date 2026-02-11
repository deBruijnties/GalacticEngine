#include <GLFW/glfw3.h>
#include "Window.h"
#include <iostream>

namespace GalacticEngine
{

    static GLFWwindow* window;

    Window::Window(int width, int height, const char* Title)
    {
        // innit GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        // create window
        window = glfwCreateWindow(width, height, Title, nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        Size.x = width;
        Size.y = height;
    }

    Window::~Window()
    {
        Release();
    }

    void Window::SetTitle(const char* Title)
    {
        glfwSetWindowTitle(window, Title);
    }

    void Window::PollEvents()
    {
        glfwSwapBuffers(window); // TEMP lookinto how vulkan does that
        glfwPollEvents();
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    void Window::Release()
    {
        std::cout << "Release GLFW\n";
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        glfwTerminate();
    }

    Vector2Int Window::GetSize()
    {
        return Size;
    }

}