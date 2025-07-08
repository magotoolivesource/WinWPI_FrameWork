#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <algorithm>
#include <cmath>

class Transform {
private:
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    Gdiplus::PointF localPosition = {0, 0};
    float localRotation = 0.0f; // in degrees
    Gdiplus::PointF localScale = {1, 1};
    float depth = 0.0f;

    bool isDirty = true;

    Gdiplus::PointF cachedWorldPosition = {0, 0};
    float cachedWorldRotation = 0.0f;
    Gdiplus::PointF cachedWorldScale = {1, 1};
    float cachedWorldDepth = 0.0f;

    Gdiplus::Matrix cachedWorldMatrix;

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

    void SetLocalRotation(float angleDegrees) {
        localRotation = angleDegrees;
        MarkDirty();
    }

    void SetLocalScale(Gdiplus::PointF scale) {
        localScale = scale;
        MarkDirty();
    }

    void SetDepth(float d) {
        depth = d;
        MarkDirty();
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
        UpdateIfDirty();
        return cachedWorldPosition;
    }

    float GetWorldRotation() {
        UpdateIfDirty();
        return cachedWorldRotation;
    }

    Gdiplus::PointF GetWorldScale() {
        UpdateIfDirty();
        return cachedWorldScale;
    }

    float GetWorldDepth() {
        UpdateIfDirty();
        return cachedWorldDepth;
    }

    Gdiplus::Matrix GetWorldMatrix() {
        UpdateIfDirty();
        return cachedWorldMatrix;
    }

private:
    void UpdateIfDirty() {
        if (!isDirty) return;

        if (parent) {
            cachedWorldPosition = parent->GetWorldPosition();
            cachedWorldPosition.X += localPosition.X;
            cachedWorldPosition.Y += localPosition.Y;

            cachedWorldRotation = parent->GetWorldRotation() + localRotation;

            auto parentScale = parent->GetWorldScale();
            cachedWorldScale.X = parentScale.X * localScale.X;
            cachedWorldScale.Y = parentScale.Y * localScale.Y;

            cachedWorldDepth = parent->GetWorldDepth() + depth;
        } else {
            cachedWorldPosition = localPosition;
            cachedWorldRotation = localRotation;
            cachedWorldScale = localScale;
            cachedWorldDepth = depth;
        }

        // Build transformation matrix
        Gdiplus::Matrix matrix;
        matrix.Translate(cachedWorldPosition.X, cachedWorldPosition.Y);
        matrix.Rotate(cachedWorldRotation);
        matrix.Scale(cachedWorldScale.X, cachedWorldScale.Y);
        cachedWorldMatrix = matrix;

        isDirty = false;
    }
};