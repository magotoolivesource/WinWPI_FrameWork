#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <algorithm>

class Transform {
private:
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    Gdiplus::PointF localPosition = {0, 0};
    float depth = 0.0f;

    Gdiplus::PointF cachedWorldPosition = {0, 0};
    float cachedWorldDepth = 0.0f;
    bool isDirty = true;

    void MarkDirty() {
        isDirty = true;
        for (auto child : children) {
            child->MarkDirty();
        }
    }

public:
    void SetLocalPosition(Gdiplus::PointF pos) {
        localPosition = pos;
        MarkDirty();
    }

    Gdiplus::PointF GetLocalPosition() const {
        return localPosition;
    }

    void SetDepth(float d) {
        depth = d;
        MarkDirty();
    }

    float GetDepth() const {
        return depth;
    }

    void SetParent(Transform* newParent) {
        if (parent) {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        parent = newParent;
        if (newParent) {
            newParent->children.push_back(this);
        }

        MarkDirty();
    }

    Gdiplus::PointF GetWorldPosition() {
        if (isDirty) {
            if (parent) {
                auto p = parent->GetWorldPosition();
                cachedWorldPosition = { p.X + localPosition.X, p.Y + localPosition.Y };
            } else {
                cachedWorldPosition = localPosition;
            }
            isDirty = false;
        }
        return cachedWorldPosition;
    }

    float GetWorldDepth() {
        if (isDirty) {
            if (parent) {
                cachedWorldDepth = parent->GetWorldDepth() + depth;
            } else {
                cachedWorldDepth = depth;
            }
        }
        return cachedWorldDepth;
    }
};