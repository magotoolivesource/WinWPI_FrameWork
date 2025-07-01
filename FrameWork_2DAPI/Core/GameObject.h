#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

#include "../Compoment/Component.h"

//class Component;

class GameObject
{
public:
    //std::string name;
    ////std::vector<std::unique_ptr<Component>> components;
    //std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

    //template<typename T, typename... Args>
    //T* AddComponent(Args&&... args) {
    //    T* comp = new T(std::forward<Args>(args)...);
    //    comp->gameObject = this;
    //    components.emplace_back(comp);
    //    return comp;
    //}

    ////void Start() {
    ////    for (auto& c : components) c->Start();
    ////}

    ////void Update() {
    ////    for (auto& c : components) c->Update();
    ////}

    //template <typename T>
    //T* GetComponent() {
    //    auto it = components.find(std::type_index(typeid(T)));
    //    return (it != components.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
    //}

    //void Start() {
    //    for (auto& [_, comp] : components) comp->Start();
    //}
    //void Update(float dt) {
    //    for (auto& [_, comp] : components) comp->Update(dt);
    //}
    //void Render(HDC hdc) {
    //    for (auto& [_, comp] : components) comp->Render(hdc);
    //}


    std::string name;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    // 새로 추가
    std::unordered_set<std::string> tags;

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        T* comp = new T(std::forward<Args>(args)...);
        comp->owner = this;
        components[std::type_index(typeid(T))].reset(comp);
        return comp;
    }

    template <typename T>
    T* GetComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        return (it != components.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    void Start() {
        //for (auto& [_, comp] : components) comp->Start();
        for(auto& comp : components) 
        {
            comp.second->Start();
		}
    }
    void Update(float dt) {
        //for (auto& [_, comp] : components) comp->Update(dt);
        for (auto& comp : components)
        {
            comp.second->Update(dt);
        }
    }
    void Render(HDC hdc) {
        //for (auto& [_, comp] : components) comp->Render(hdc);
        for (auto& comp : components)
        {
            comp.second->Render(hdc);
        }
    }

    void AddTag(const std::string& tag) {
        tags.insert(tag);
    }

    bool HasTag(const std::string& tag) const {
        return tags.find(tag) != tags.end();
    }
};

