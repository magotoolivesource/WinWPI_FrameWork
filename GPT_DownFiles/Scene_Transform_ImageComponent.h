#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "Transform.h"
#include "ImageComponent.h"

class GameObject;

class Scene {
private:
    std::unordered_set<GameObject*> objects;

public:
    void AddObject(GameObject* obj) {
        objects.insert(obj);
    }

    void DestroyObject(GameObject* obj) {
        if (objects.find(obj) != objects.end()) {
            delete obj;
            objects.erase(obj);
        }
    }

    void Draw(HDC hdc) {
        std::vector<GameObject*> sortedObjects(objects.begin(), objects.end());
        std::sort(sortedObjects.begin(), sortedObjects.end(), [](GameObject* a, GameObject* b) {
            return a->GetTransform()->GetWorldDepth() < b->GetTransform()->GetWorldDepth();
        });

        for (auto obj : sortedObjects) {
            obj->Draw(hdc);
        }
    }
};

class Transform {
private:
    Transform* parent = nullptr;
    std::vector<Transform*> children;

public:
    Gdiplus::PointF localPosition = {0, 0};
    float depth = 0.0f;

    void SetParent(Transform* newParent) {
        if (parent) {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        parent = newParent;
        if (newParent) {
            newParent->children.push_back(this);
        }
    }

    Gdiplus::PointF GetWorldPosition() const {
        if (!parent) return localPosition;
        auto p = parent->GetWorldPosition();
        return {p.X + localPosition.X, p.Y + localPosition.Y};
    }

    float GetWorldDepth() const {
        if (!parent) return depth;
        return parent->GetWorldDepth() + depth;
    }
};

class GameObject {
private:
    Transform* transform;
    ImageComponent* image;

public:
    GameObject() {
        transform = new Transform();
        image = nullptr;
    }

    ~GameObject() {
        if (image) delete image;
        delete transform;
    }

    Transform* GetTransform() { return transform; }

    template<typename T>
    T* AddComponent() {
        if constexpr (std::is_same<T, ImageComponent>::value) {
            image = new ImageComponent();
            image->SetTransform(transform);
            return static_cast<T*>(image);
        }
        return nullptr;
    }

    void Draw(HDC hdc) {
        if (image) image->Draw(hdc);
    }
};