#pragma once
#include "UIElement.h"
#include "core/Rendering/Resources/Texture.h"
#include "core/Rendering/Resources/Material.h" // Your engine renderer
#include "core/Rendering/Mesh/FullScreenQuad.h" // Your engine renderer

class TextureElement : public UIElement
{
public:
    static void Init();

    TextureElement(Texture* tex) : texture(tex) {}

    void Render() override;


private:
    static Material* material;
    Texture* texture = nullptr;
};