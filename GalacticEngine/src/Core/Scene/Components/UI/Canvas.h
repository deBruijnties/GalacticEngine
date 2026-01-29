#pragma once
#include <Core/Scene/Component.h>

class Canvas : public Component
{
public:
    enum class RenderMode
    {
        ScreenSpace, // Drawn directly in screen space (HUD, menus)
        WorldSpace   // Exists in the 3D world (VR panels, in-world screens)
    };

    RenderMode renderMode = RenderMode::ScreenSpace;

    enum class ScaleMode
    {
        // Size is always the same in screen pixels, regardless of resolution.
        // Example: 32px is always 32px on screen.
        ConstantPixelSize,

        // Size scales proportionally with the screen resolution.
        // Example: UI designed for 1920x1080 scales up/down to other resolutions.
        ScaleWithScreenSize,
    };

	// Only used if renderMode == ScreenSpace couse scaling with screen size in world space is weird
    ScaleMode scaleMode = ScaleMode::ScaleWithScreenSize;

    // Reference resolution for ScaleWithScreenSize like Unity
    int referenceWidth = 1920;
    int referenceHeight = 1080;

    void OnResize(int width, int height) override;
    void OnStart() override;



protected:
    Matrix4 uiProjectionMatrix;
	Camera* camera = nullptr;
private:
    void ReCalculateMatrix();
};
