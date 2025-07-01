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



public:
    GameObject();
    virtual ~GameObject();

    std::string name;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    // 새로 추가
    std::unordered_set<std::string> tags;

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args) {

   //     if (typeid(T) == typeid(Transform))
   //     {
			//return nullptr; // Transform은 GameObject에 자동으로 추가되므로, 명시적으로 추가하지 않음
   //     }

        T* comp = new T(std::forward<Args>(args)...);
        comp->owner = this;
		comp->Initialize(); // 컴포넌트 초기화 호출 추가
        components[std::type_index(typeid(T))].reset(comp);
        return comp;
    }

    template <typename T, typename... Args>
    bool RemoveComponent() {
        //T* comp = new T(std::forward<Args>(args)...);
        //comp->owner = this;

        //auto it = components.find(std::type_index(typeid(T)));

        if (components[std::type_index(typeid(T))])
        {
            delete components[std::type_index(typeid(T))].get();
            components[std::type_index(typeid(T))].release();// = nullptr;

            components.erase(std::type_index(typeid(T)));

            return true;
        }

        

        return false;
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

protected:
    void Initialize();
};

