#include "pch.h"
#include "Canvas.h"
#include <Core/engine.h>
#include <Core/Scene/Components/Camera.h>
#include <glm/gtc/matrix_transform.hpp>



const glm::mat4& Canvas::GetProjection() const
{
    static glm::mat4 ortho;

    switch (renderMode)
    {
        case RenderMode::ScreenSpaceOverlay:
        {
            ortho = glm::ortho(
                0.0f, (float)Engine::width,
                (float)Engine::height, 0.0f,
                -1.0f, 1.0f
            );
            return ortho;
        }

        case RenderMode::ScreenSpaceCamera:
        {
            if (targetCamera)
            {
                glm::ivec2 res = targetCamera->GetResolution();
                ortho = glm::ortho( 
                    0.0f, (float)res.x,
                    (float)res.y, 0.0f,
                    -1.0f, 1.0f
                );
                return ortho;
            }
        }

        case RenderMode::WorldSpace:
        {
            glm::ivec2 res = targetCamera->GetResolution();
            ortho = glm::ortho(
                0.0f, (float)res.x,
                (float)res.y, 0.0f,
                -1.0f, 1.0f
            );
            return ortho;
        }
    }

    return ortho; // fallback
}


const glm::mat4& Canvas::GetView(const Camera* cam) const
{
    static glm::mat4 identity(1.0f);

    if (renderMode == RenderMode::ScreenSpaceOverlay)
        return identity;

    if (renderMode == RenderMode::ScreenSpaceCamera)
        return targetCamera->transform->worldMatrix;

    // World-space
    return targetCamera->transform->worldMatrix;
}

