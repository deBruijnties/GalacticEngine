#include "RectTransform.h"
#include <Core/engine.h>
#include <Core/Scene/GameObject.h>
#include <Core/Scene/Components/UI/Canvas.h>
#include <glm/gtc/matrix_transform.hpp>

void RectTransform::UpdateMatrices()
{
    if (!dirty)
        return;
    dirty = false;

    glm::vec2 parentSize;

    if (parent && parent->IsUI())
    {
        auto* p = static_cast<RectTransform*>(parent);
        parentSize = p->size;
    }
    else
    {
        parentSize = { Engine::width, Engine::height };
    }

    // Anchor calculation
    glm::vec2 anchorCenter =
        glm::mix(anchorMin, anchorMax, 0.5f) * parentSize;

    glm::vec2 pivotOffset = size * pivot;

    glm::vec2 finalPos =
        anchorCenter + anchoredPosition - pivotOffset;

    localPosition = { finalPos.x, finalPos.y, 0.0f };
    localRotation = glm::quat(1, 0, 0, 0);
    localScale = { 1, 1, 1 };

    const bool isCanvas = gameObject->getComponent<Canvas>() != nullptr;

    // Build local matrix
    if (isCanvas)
    {
        // Canvas defines coordinate space only
        localMatrix =
            glm::translate(glm::mat4(1.0f), localPosition);
    }
    else
    {
        // UI element applies pixel size
        localMatrix =
            glm::translate(glm::mat4(1.0f), localPosition) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    }

    // Build world matrix
    if (parent)
        worldMatrix = parent->worldMatrix * localMatrix;
    else
        worldMatrix = localMatrix;

    // Update children
    for (Transform* child : GetChildren())
    {
        child->MarkDirty();
        child->UpdateMatrices();
    }
}

