#pragma once
#include "core/Math/Math.h"
#include "core/Rendering/Buffers/framebuffer.h"
#include "core/Rendering/Resources/Material.h"
#include "UIElement.h"
#include <vector>

class Canvas
{
public:
    Canvas(int width = 1920, int height = 1080)
    {
        overlay = new FrameBuffer();
        overlay->init(width, height);

        projectionMatrix = Math::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
    }

    ~Canvas()
    {
        for (UIElement* e : Elements)
            delete e;
        delete overlay;
    }

    void Render()
    {
        // Recompute orthographic matrix to handle window resizing
        projectionMatrix = Math::ortho(0.0f, (float)Engine::width, 0.0f, (float)Engine::height, -1.0f, 1.0f);

        // Bind framebuffer if you want overlay effects
        // overlay->bind();

        // Set global orthographic projection for all UI materials
        Material::SetGlobalMat4("u_Projection", projectionMatrix);

        for (UIElement* element : Elements)
        {
            element->Render();
        }

        // overlay->unbind();
    }

    void AddElement(UIElement* element)
    {
        Elements.push_back(element);
    }

private:
    FrameBuffer* overlay = nullptr;
    Matrix4 projectionMatrix;

public:
    std::vector<UIElement*> Elements;
};