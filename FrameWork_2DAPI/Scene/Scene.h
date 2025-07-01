#pragma once
#include <vector>
#include <memory>
#include <string>

//class GameObject;
#include "../Core/GameObject.h"

class Scene
{
public:
    std::vector<std::unique_ptr<GameObject>> objects;

    GameObject* CreateObject(const std::string& name) {
        auto obj = std::make_unique<GameObject>();
        obj->name = name;
        GameObject* ptr = obj.get();
        objects.emplace_back(std::move(obj));
        return ptr;
    }

    void Start() 
    {
        for (auto& obj : objects) 
            obj->Start();
    }

    void Update(float dt)
    {
        for (auto& obj : objects) 
            obj->Update(dt);
    }

    void Render(HDC p_hdc) 
    {
        for (auto& obj : objects) 
            obj->Render(p_hdc);
	}
};

