#include <pch.h>
#include "Gameobject.h"
#include "Scene.h"
#include "Core/engine.h"
#include "Components/Camera.h"



GameObject* Scene::createObject(const std::string& name)
{
    auto obj = std::make_unique<GameObject>(name);
    GameObject* ptr = obj.get();
    objects.push_back(std::move(obj));
    return ptr;
}


void Scene::Start()
{
    for (auto& obj : objects)
        obj->start();
}

void Scene::Update()
{
    for (auto& obj : objects)
        obj->update();
}

void Scene::Render()
{
    for (auto& obj : objects)
        obj->submitRender();
}

void Scene::resize(int Width, int Height)
{
    for (auto& obj : objects)
        obj->resize(Width, Height);
}