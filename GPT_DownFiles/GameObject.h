#pragma once
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <iostream>
#include "Transform.h"

class GameObject {
private:
    std::unordered_map<std::type_index, Component*> components;
    bool active = true;

public:
    GameObject() {
        // Transform 생성 후 자동 추가
        Transform* t = new Transform();
        t->gameObject = this;
        components[typeid(Transform)] = t;
    }

    ~GameObject() {
        for (auto& pair : components) {
            delete pair.second;
        }
    }

    void SetActive(bool a) { active = a; }
    bool IsActive() const { return active; }

    template<typename T>
    T* AddComponent() {
        if constexpr (std::is_same<T, Transform>::value) {
            std::cerr << "[⚠️ 경고] Transform은 자동 생성되므로 추가하지 마세요.
";
            return GetComponent<T>();
        }

        T* comp = new T();
        comp->gameObject = this;
        components[typeid(T)] = comp;
        return comp;
    }

    template<typename T>
    T* GetComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }

    Transform* GetTransform() {
        return GetComponent<Transform>();
    }
};