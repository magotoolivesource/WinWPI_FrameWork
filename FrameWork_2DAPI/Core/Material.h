#pragma once
#include <windows.h>
#include <gdiplus.h>
//#include <gdiplustypes.h>


class Material {
public:
    Gdiplus::Image* image;
    Gdiplus::Rect uvRect;

    Material(Gdiplus::Image* img = nullptr)
        : image(img), uvRect(0, 0, 0, 0) {}

    void SetUV(int left, int top, int width, int height);
};