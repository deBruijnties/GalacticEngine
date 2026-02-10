#pragma once
class Window
{
public:
    
    Window(int width, int height, const char* Title = "");

    ~Window();

    void SetTitle(const char* Title = "");

    void PollEvents();

    bool ShouldClose();

    void Release();

private:
    
};

