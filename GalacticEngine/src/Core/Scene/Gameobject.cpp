#include "pch.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "Component.h"

GameObject::GameObject(const std::string& name)
    : name(name)
{
    // Create default Transform
    auto t = std::make_unique<Transform>();
    t->gameObject = this;

    transform = t.get();
    components.push_back(std::move(t));
}

void GameObject::start()
{
    if (started) return;

    for (auto& c : components)
        c->OnStart();

    started = true;
}

void GameObject::update()
{
    if (!active) return;
    if (!started) start();

    // Update Transform first
    for (auto& c : components)
        if (dynamic_cast<Transform*>(c.get()))
            c->OnUpdate();

    // Then everything else
    for (auto& c : components)
        if (!dynamic_cast<Transform*>(c.get()))
            c->OnUpdate();
}

void GameObject::submitRender()
{
    if (!active /* || Engine::UIPass != transform->IsUI()*/) return;

    for (std::unique_ptr<Component>& c : components)
        c->OnSubmitRender();
}

void GameObject::resize(int width, int height)
{
    if (!active) return;

    for (auto& c : components)
        c->OnResize(width, height);
}
