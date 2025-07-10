#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Transform.h"
#include "Camera.h"
#include "Material.h"

class Viewport {
private:
    Gdiplus::RectF viewRect;

public:
    void SetViewRect(float x, float y, float width, float height) {
        viewRect = Gdiplus::RectF(x, y, width, height);
    }

    bool IsVisible(const Gdiplus::PointF& screenPos, float width, float height) const {
        Gdiplus::RectF objectRect(screenPos.X, screenPos.Y, width, height);
        return viewRect.IntersectsWith(objectRect);
    }

    const Gdiplus::RectF& GetViewRect() const {
        return viewRect;
    }
};

class Collider {
private:
    Transform* transform = nullptr;
    float width = 0;
    float height = 0;

public:
    void SetTransform(Transform* t) { transform = t; }
    void SetSize(float w, float h) { width = w; height = h; }

    Gdiplus::RectF GetWorldBounds() const {
        if (!transform) return Gdiplus::RectF();
        auto pos = transform->GetWorldPosition();
        return Gdiplus::RectF(pos.X, pos.Y, width, height);
    }

    bool Intersects(const Collider& other) const {
        return GetWorldBounds().IntersectsWith(other.GetWorldBounds());
    }
};

class ImageComponent {
private:
    Transform* transform = nullptr;
    Material* material = nullptr;
    Collider* collider = nullptr;
    float width = 0;
    float height = 0;

public:
    void SetTransform(Transform* t) {
        transform = t;
        if (!collider) {
            collider = new Collider();
            collider->SetTransform(t);
        }
    }

    void SetMaterial(Material* mat) {
        material = mat;
    }

    void SetSize(float w, float h) {
        width = w;
        height = h;
        if (collider) {
            collider->SetSize(w, h);
        }
    }

    void Draw(HDC hdc, const Camera& camera, const Viewport& viewport) {
        if (!transform || !material || !collider) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighSpeed);

        Gdiplus::RectF bounds = collider->GetWorldBounds();
        Gdiplus::PointF screenPos = camera.ApplyToPoint({bounds.X, bounds.Y});

        if (!viewport.IsVisible(screenPos, bounds.Width, bounds.Height))
            return;

        Gdiplus::Matrix worldMatrix = transform->GetWorldMatrix();
        camera.ApplyToMatrix(worldMatrix);
        graphics.SetTransform(&worldMatrix);

        graphics.DrawImage(
            material->GetImage(),
            Gdiplus::RectF(screenPos.X, screenPos.Y, width, height),
            material->GetSourceRect().X,
            material->GetSourceRect().Y,
            material->GetSourceRect().Width,
            material->GetSourceRect().Height,
            Gdiplus::UnitPixel
        );
    }

    Collider* GetCollider() const { return collider; }
};