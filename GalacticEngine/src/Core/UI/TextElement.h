#pragma once
#include "UIElement.h"
#include <string>
#include "core/Rendering/Renderer.h"

class TextElement : public UIElement
{
public:
    TextElement(const std::string& txt) : text(txt) {}

    void Render() override
    {
        std::cout << "Render TextElement\n";
        // Stub: just placeholder
        // Later: implement text rendering using bitmap font or font system
        // Example: Renderer::DrawText(text, Position, Size);
    }

    void SetText(const std::string& txt) { text = txt; }

private:
    std::string text;
};