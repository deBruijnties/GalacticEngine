#pragma once
#include <string>
#include <vector>
#include <memory>

class Transform;
class Component;

class GameObject {
public:
    std::string name;
    bool active = true;
    bool started = false;
    Transform* transform = nullptr;

    GameObject(const std::string& name);


    template<typename T>
    T* setTransform() {
        static_assert(std::is_base_of<Transform, T>::value);

        // Remove old transform only
        for (auto it = components.begin(); it != components.end(); ++it) {
            if (dynamic_cast<Transform*>(it->get())) {
                components.erase(it);
                break;
            }
        }

        auto t = std::make_unique<T>();
        t->gameObject = this;
        transform = t.get();
        components.insert(components.begin(), std::move(t));

        // Update existing components' transform pointer
        for (auto& c : components)
            c->transform = transform;

        return static_cast<T*>(transform);
    }

    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        static_assert(!std::is_base_of<Transform, T>::value,
            "ERROR: Use setTransform<T>() for Transform components!");

        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->gameObject = this;
        comp->transform = transform;
        T* ptr = comp.get();
        components.push_back(std::move(comp));
        return ptr;
    }


    template<typename T>
    T* getComponent() {
        for (auto& c : components) {
            if (auto ptr = dynamic_cast<T*>(c.get()))
                return ptr;
        }
        return nullptr;
    }

    // - Non-template functions -
    void start();
    void update();
    void submitRender();
    void resize(int width, int height);

private:
    std::vector<std::unique_ptr<Component>> components;
};
