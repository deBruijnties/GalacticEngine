#pragma once

class GameObject;
class Transform;

class Component {
public:
    GameObject* gameObject = nullptr;
    Transform* transform = nullptr;

    Component() = default;
    virtual ~Component() {}

    // Called once when the object becomes active
    virtual void OnStart() {}

    // Called every frame
    virtual void OnUpdate() {}

    // Called every render pass
    virtual void OnSubmitRender() {}

    virtual void OnResize(int Width, int Height) {}
};
