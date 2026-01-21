#include "UIImage.h"
#include <Core/Scene/GameObject.h>
#include <Core/Scene/Components/RectTransform.h>
#include <Core/Scene/Components/UI/Canvas.h>
#include "UIMesh.h"
#include <glad/glad.h>
#include <iostream>

void PrintMat4(const glm::mat4& m, const std::string& name = "");

void UIImage::OnRender()
{
    if (!material)
        return;

    auto* rect = gameObject->getComponent<RectTransform>();
    if (!rect)
        return;

    // Find Canvas in parents
    Canvas* canvas = nullptr;
    Transform* t = rect;

    while (t)
    {
        canvas = t->gameObject->getComponent<Canvas>();
        if (canvas)
            break;
        t = t->GetParent();
    }

    if (!canvas)
        return;

    if (canvas->targetCamera)
    {
		std::cout << "UIImage rendering with Camera target\n";
        if (canvas->targetCamera->GetOutputColor())
        {
			std::cout << "Using camera output texture for UI rendering\n";
            canvas->targetCamera->GetOutputColor()->bind();
        }
    }

    // --- UI render state ---
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    material->bind();
    material->setMat4("model", rect->worldMatrix);
    material->setMat4("projection", canvas->GetProjection());

    PrintMat4(rect->worldMatrix, "model");
    PrintMat4(canvas->GetProjection(), "projection");

    UIMesh::GetQuad()->Draw();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    if (canvas->targetCamera)
    {
        if (canvas->targetCamera->GetOutputColor())
        {
            canvas->targetCamera->GetOutputColor()->unbind();
        }
    }
}
