#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "Transform.h"

#pragma comment(lib, "gdiplus.lib")

class ImageComponent {
private:
    Gdiplus::Image* image;
    Transform* transform;
    int drawWidth, drawHeight;
    RECT drawRect;
    bool useDrawRect;

public:
    ImageComponent()
        : image(nullptr), transform(nullptr), drawWidth(0), drawHeight(0), useDrawRect(false)
    {
        drawRect = {0, 0, 0, 0};
    }

    ~ImageComponent() {
        if (image) {
            delete image;
        }
    }

    void SetTransform(Transform* t) {
        transform = t;
    }

    bool LoadImage(const std::wstring& path) {
        if (image) {
            delete image;
        }
        image = new Gdiplus::Image(path.c_str());
        return image && image->GetLastStatus() == Gdiplus::Ok;
    }

    void SetSize(int width, int height) {
        drawWidth = width;
        drawHeight = height;
    }

    void SetDrawRect(int left, int top, int right, int bottom) {
        drawRect = { left, top, right, bottom };
        useDrawRect = true;
    }

    void Draw(HDC hdc) {
        if (!image || !transform) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

        Gdiplus::PointF pos(transform->position.x, transform->position.y);

        graphics.TranslateTransform(pos.X, pos.Y);
        graphics.RotateTransform(transform->rotation);
        graphics.TranslateTransform(-pos.X, -pos.Y);

        int x = static_cast<int>(transform->position.x);
        int y = static_cast<int>(transform->position.y);
        int w = drawWidth > 0 ? drawWidth : image->GetWidth();
        int h = drawHeight > 0 ? drawHeight : image->GetHeight();

        if (useDrawRect) {
            Gdiplus::Rect destRect(x, y, w, h);
            Gdiplus::Rect srcRect(drawRect.left, drawRect.top,
                                  drawRect.right - drawRect.left,
                                  drawRect.bottom - drawRect.top);
            graphics.DrawImage(image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
        } else {
            graphics.DrawImage(image, x, y, w, h);
        }

        graphics.ResetTransform();
    }
};