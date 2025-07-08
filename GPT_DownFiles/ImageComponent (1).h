#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Transform.h"
#include "Material.h"

#pragma comment(lib, "gdiplus.lib")

class ImageComponent {
private:
    Transform* transform;
    Material* material;
    int drawWidth, drawHeight;

public:
    ImageComponent()
        : transform(nullptr), material(nullptr), drawWidth(0), drawHeight(0) {}

    void SetTransform(Transform* t) {
        transform = t;
    }

    void SetMaterial(Material* mat) {
        material = mat;
    }

    void SetSize(int width, int height) {
        drawWidth = width;
        drawHeight = height;
    }

    void Draw(HDC hdc) {
        if (!material || !material->image || !transform) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

        Gdiplus::PointF pos(transform->position.x, transform->position.y);

        graphics.TranslateTransform(pos.X, pos.Y);
        graphics.RotateTransform(transform->rotation);
        graphics.TranslateTransform(-pos.X, -pos.Y);

        int x = static_cast<int>(transform->position.x);
        int y = static_cast<int>(transform->position.y);

        Gdiplus::Rect destRect(x, y, drawWidth, drawHeight);

        if (material->uvRect.Width > 0 && material->uvRect.Height > 0) {
            graphics.DrawImage(material->image, destRect,
                material->uvRect.X, material->uvRect.Y,
                material->uvRect.Width, material->uvRect.Height,
                Gdiplus::UnitPixel);
        } else {
            graphics.DrawImage(material->image, x, y, drawWidth, drawHeight);
        }

        graphics.ResetTransform();
    }
};