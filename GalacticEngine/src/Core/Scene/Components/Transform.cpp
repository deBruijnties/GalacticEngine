#include "pch.h"
#include "Transform.h"
#include <iostream>
#include <core/engine.h>
#include "../Gameobject.h"


void Transform::AddChild(Transform* child)
{
    children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}


void Transform::SetParent(Transform* newParent, bool keepWorld)
{
    if (newParent == parent) return;

    UpdateMatrices(); // ensure worldMatrix is valid BEFORE changing parent

    if (parent)
        parent->RemoveChild(this);

    if (keepWorld)
    {
        Matrix4 desiredWorld = worldMatrix;

        if (newParent)
            localMatrix = GalacticEngine::Math::Inverse(newParent->worldMatrix) * desiredWorld;
        else
            localMatrix = desiredWorld;

        Math::DecomposeMatrix(localMatrix, localPosition, localRotation, localScale);
    }

    parent = newParent;

    if (newParent)
        newParent->AddChild(this);

    MarkDirty();
}

Transform* Transform::GetChild(std::string Name)
{
    for (Transform* i : children)
    {
        if (i->gameObject->name == Name)
            return i;
    }
	return nullptr;
}

const std::vector<Transform*>& Transform::GetChildren() const
{
    return children;
}



void Transform::OnUpdate() {UpdateMatrices(); }

void Transform::MarkDirty()
{
    dirty = true;
	//std::cout << gameObject->name << " marked dirty\n";
}


void Transform::UpdateMatrices()
{
    if (!dirty) return;
    dirty = false;

    localMatrix = Matrix4::Identity();
    localMatrix = Math::Translate(localMatrix, localPosition);
    localMatrix = Math::Rotate(localMatrix, localRotation);
    localMatrix = Math::Scale(localMatrix, localScale);


    if (parent)
        worldMatrix = parent->worldMatrix * localMatrix;
    else
        worldMatrix = localMatrix;

    Math::DecomposeMatrix(worldMatrix, worldPosition, worldRotation, worldScale);

    for (Transform* child : children)
    {
        child->MarkDirty();
        child->UpdateMatrices();
    }
}