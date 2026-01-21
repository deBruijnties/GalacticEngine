#pragma once
#include "Transform.h"

class RectTransform : public Transform
{
public:
    // Size in pixels (before Canvas scaling)
    glm::vec2 size{ 100, 100 };

    // Anchors in normalized parent rect space
    glm::vec2 anchorMin{ 0.5f, 0.5f };
    glm::vec2 anchorMax{ 0.5f, 0.5f };

    // Offset from anchor reference point (pixels)
    glm::vec2 anchoredPosition{ 0, 0 };

    // Pivot inside this rect (0–1)
    glm::vec2 pivot{ 0.5f, 0.5f };

    bool IsUI() const override { return true; }

    void UpdateMatrices() override;
};
