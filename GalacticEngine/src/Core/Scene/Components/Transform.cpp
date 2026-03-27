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
    // erase-remove idiom - cleaner than a manual loop
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Transform::SetParent(Transform* newParent, bool keepWorld)
{
    if (newParent == parent) return;

    // make sure our worldMatrix reflects current local values before we start
    // reparenting with a stale worldMatrix would corrupt the keepWorld calculation below
    UpdateMatrices();

    if (parent)
        parent->RemoveChild(this);

    if (keepWorld)
    {
        // figure out what local transform we'd need under the new parent to end up
        // at the same world position/rotation/scale we have right now
        Matrix4 desiredWorld = worldMatrix;
        if (newParent)
            localMatrix = GalacticEngine::Math::Inverse(newParent->worldMatrix) * desiredWorld;
        else
            localMatrix = desiredWorld; // no parent, local == world
        Math::DecomposeMatrix(localMatrix, localPosition, localRotation, localScale);
    }

    parent = newParent;

    if (newParent)
        newParent->AddChild(this);

    MarkDirty(); // matrices need to be rebuilt under the new parent
}

// walks direct children only, not the full hierarchy
Transform* Transform::GetChild(std::string Name)
{
    for (Transform* i : children)
    {
        if (i->gameObject->name == Name)
            return i;
    }
    return nullptr; // not found
}

const std::vector<Transform*>& Transform::GetChildren() const
{
    return children;
}

void Transform::OnUpdate()
{
    UpdateMatrices();
}

// call this whenever localPosition, localRotation, or localScale changes
// defers the matrix rebuild until UpdateMatrices is actually called
void Transform::MarkDirty()
{
    dirty = true;
}

void Transform::UpdateMatrices()
{
    if (!dirty) return; // nothing changed, skip the rebuild
    dirty = false;

    // rebuild local TRS matrix from components
    localMatrix = Matrix4::Identity();
    localMatrix = Math::Translate(localMatrix, localPosition);
    localMatrix = Math::Rotate(localMatrix, localRotation);
    localMatrix = Math::Scale(localMatrix, localScale);

    // world matrix is local space lifted into parent's space, or just local if there's no parent
    worldMatrix = parent ? parent->worldMatrix * localMatrix : localMatrix;

    // decompose so worldPosition/worldRotation/worldScale stay in sync and are readable directly
    Math::DecomposeMatrix(worldMatrix, worldPosition, worldRotation, worldScale);

    // propagate the dirty flag down the hierarchy - children's world matrices depend on ours
    for (Transform* child : children)
    {
        child->MarkDirty();
        child->UpdateMatrices();
    }
}