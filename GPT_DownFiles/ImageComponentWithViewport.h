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

class ImageComponent {
private:
    Transform* transform = nullptr;
    Material* material = nullptr;
    float width = 0;
    float height = 0;

public:
    void SetTransform(Transform* t) {
        transform = t;
    }

    void SetMaterial(Material* mat) {
        material = mat;
    }

    void SetSize(float w, float h) {
        width = w;
        height = h;
    }

    void Draw(HDC hdc, const Camera& camera, const Viewport& viewport) {
        if (!transform || !material) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighSpeed);

        auto worldPos = transform->GetWorldPosition();
        auto screenPos = camera.ApplyToPoint(worldPos);

        if (!viewport.IsVisible(screenPos, width, height)) return;

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
};