#pragma once
#include <vector>
#include <memory>
#include <string>

class GameObject;
class Camera;

class Scene {
public:
    std::vector<std::unique_ptr<GameObject>> objects;

    GameObject* createObject(const std::string& name);

    void Start();

    void Update();

    void Render();
   
    void resize(int Width, int Height);
};
