#include "pch.h"
#include "TextureElement.h"

Material* TextureElement::material = nullptr;

void TextureElement::Init()
{
    material = new Material(new Shader(
        "engineassets/shaders/UI/TextureShader/TextureShader.vert",
        "engineassets/shaders/UI/TextureShader/TextureShader.frag",
        true
    ));
}

void TextureElement::Render()
{
    if (!texture || !material)
        return;

    // Get texture size
    float texWidth = (float)texture->Width;
    float texHeight = (float)texture->Height;
    float aspect = texWidth / texHeight;

    // Desired max size (relative to screen, e.g., quarter of height)
    float maxHeight = Engine::height / 4.0f;
    float maxWidth = maxHeight * aspect; // maintain aspect ratio

    Vector2 size = Vector2(maxWidth, maxHeight);

    // Center of screen
    Vector2 centerPos = Vector2(Engine::width / 2.0f, Engine::height / 2.0f);

    // Optional offset
    Vector2 offset = Vector2(0.0f, 0.0f);

    // Final position
    Vector2 finalPos = centerPos + offset;

    // Model matrix: Scale -> Rotate -> Translate
    Matrix4 scale = Math::Scale(Matrix4::Identity(), Vector3(size.x, size.y, 1.0f));
    Matrix4 rotation = Matrix4::Identity(); // no rotation
    Matrix4 translate = Math::Translate(Matrix4::Identity(), Vector3(finalPos.x, finalPos.y, 0.0f));

    Matrix4 model = translate * rotation * scale;

    material->Bind();
    material->SetMat4("u_model", model);

    texture->Bind(0);
    material->SetInt("uTexture", 0);

    FullscreenQuad::Get().Draw();
}