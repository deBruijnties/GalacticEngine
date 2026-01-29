#pragma once
#include <Core/Scene/Component.h>
#include <vector>
#include <string>
#include <Core/engine.h>

class Transform : public Component
{
public:

    // Local transform
    Vector3 localPosition{ 0.f, 0.f, 0.f };
    Quaternion localRotation = Quaternion::Identity();
    Vector3 localScale{ 1.f, 1.f, 1.f };

    // World transform
    Vector3 worldPosition{ 0.f, 0.f, 0.f };
    Quaternion worldRotation = Quaternion::Identity();
    Vector3 worldScale{ 1.f, 1.f, 1.f };

    Matrix4 localMatrix = Matrix4::Identity();
    Matrix4 worldMatrix = Matrix4::Identity();

protected:
    Transform* parent = nullptr;
    std::vector<Transform*> children;
    bool dirty = true;

public:

    void AddChild(Transform* child);

    void RemoveChild(Transform* child);

    void SetParent(Transform* newParent, bool keepWorld = true);

    Transform* GetChild(std::string Name);

    const std::vector<Transform*>& GetChildren() const;
    
    Transform* GetParent() const { return parent; }

    void OnUpdate() override;

    virtual void MarkDirty();
    virtual void UpdateMatrices();

    virtual bool IsUI() const { return false; }

};
