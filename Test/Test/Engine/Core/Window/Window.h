#pragma once
#include "core/Math/Types.h"

namespace GalacticEngine
{
    class Window
    {
    public:

        Window(int width, int height, const char* Title = "");

        ~Window();

        void SetTitle(const char* Title = "");

        void PollEvents();

        bool ShouldClose();

        void Release();

        Vector2Int GetSize();

    private:
        Vector2Int Size;
    };

}