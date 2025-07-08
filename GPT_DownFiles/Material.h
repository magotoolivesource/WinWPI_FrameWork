#pragma once
#include <gdiplus.h>
#include <memory>

class Material {
public:
    std::shared_ptr<Gdiplus::Image> image;
    Gdiplus::Rect uvRect;

    Material(std::shared_ptr<Gdiplus::Image> img = nullptr)
        : image(img), uvRect(0, 0, 0, 0) {}

    void SetUV(int left, int top, int width, int height) {
        uvRect = Gdiplus::Rect(left, top, width, height);
    }
};