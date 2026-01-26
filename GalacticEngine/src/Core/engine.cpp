#include "pch.h"
#include "Engine.h"
#include <iostream>

namespace GalacticEngine::Core
{
    int Engine::width = 0;
    int Engine::height = 0;

    GLFWwindow* Engine::window = nullptr;
    Scene* Engine::currentScene = nullptr;

    Engine::Engine(int Width, int Height, const char* Title)
    {
        Shader::AddIncludeDirectory("assets/shaders/RendererShaders/Lighting");

		// Set initial width and height
        width = Width;
        height = Height;

        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        // OpenGL 4.4 core profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        window = glfwCreateWindow(width, height, Title, nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }

        // Store engine pointer for callbacks
        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        // Load OpenGL functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
            return;
        }

        Time::Initialize();
        Input::Mouse::Init(window);
        Input::Input::Init(window);


    }

    Engine::~Engine()
    {
        // Cleanup handled in Run()
    }

    void Engine::Resize(int Width, int Height)
    {
		// Resize is called with 0,0 when minimizing the window this is for crash prevention
        if (Width == 0 || Height == 0)
            return;

        width = Width;
        height = Height;
        currentScene->resize(width, height);
    }

    void Engine::Render()
    {
		PROFILE_FUNCTION();

        Renderer::BeginScene();
        currentScene->Render();
        
        for (Camera* cam : Camera::allCameras)
        {
            PROFILE_SCOPE("Camera Render");
            Renderer::BeginCamera(*cam);
            Renderer::EndCamera();
        }
        
        Renderer::EndScene();
    }


    void Engine::Run()
    {
        OnStart();
        Renderer::Init(width, height);

        double lastTime = glfwGetTime();

        while (window && !glfwWindowShouldClose(window))
        {
            PROFILE_SCOPE("Frame");
            Profiler::Get().BeginFrame();

            double currentTime = glfwGetTime();
            double dt = currentTime - lastTime;
            lastTime = currentTime;

            Time::Update(dt);
            Input::Input::Update();
            {
                PROFILE_SCOPE("input");
                glfwPollEvents();
                OnProcessInput();

            }
            {
                PROFILE_SCOPE("update");
                currentScene->Update();
                OnUpdate();
                Input::Mouse::Update();

            }
            {
                PROFILE_SCOPE("RenderSubmit");
                Render();
            }
            {
                PROFILE_SCOPE("swapbuffers");
                glfwSwapBuffers(window);
            }

            Profiler::Profiler::Get().EndFrame();
#if _DEBUG

            static double dumpTimer = 0.0;
            dumpTimer += dt;
            if (dumpTimer > 1.0)
            {
                Profiler::Profiler::Get().DumpToConsole();
                dumpTimer = 0.0;
            }
#else
            static double dumpTimer = 0.0;
            dumpTimer += dt;
            if (dumpTimer > 1.0)
            {
                std::cout << Time::fps << "fps\n";
                dumpTimer = 0;
            }
#endif
        }

        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        glfwTerminate();
    }

    void Engine::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        if (engine) {
            engine->Resize(width, height);
        }
    }
}