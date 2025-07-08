#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <cmath>
#include "Transform.h"

#pragma comment(lib, "gdiplus.lib")

class TextComponent {
private:
    Transform* transform = nullptr;
    std::wstring text = L"";
    std::wstring fontFamily = L"Arial";
    float fontSize = 24.0f;
    Gdiplus::Color fontColor = Gdiplus::Color(255, 0, 0, 0); // black
    Gdiplus::Color outlineColor = Gdiplus::Color(255, 255, 255, 255); // white
    int width = 200;
    int height = 50;
    Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentNear;
    Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;
    bool useOutline = false;
    bool useBlink = false;
    double elapsedTime = 0.0;

public:
    void SetTransform(Transform* t) {
        transform = t;
    }

    void SetText(const std::wstring& str) {
        text = str;
    }

    void SetFont(const std::wstring& family, float size) {
        fontFamily = family;
        fontSize = size;
    }

    void SetFontColor(BYTE a, BYTE r, BYTE g, BYTE b) {
        fontColor = Gdiplus::Color(a, r, g, b);
    }

    void SetOutlineColor(BYTE a, BYTE r, BYTE g, BYTE b) {
        outlineColor = Gdiplus::Color(a, r, g, b);
        useOutline = true;
    }

    void SetSize(int w, int h) {
        width = w;
        height = h;
    }

    void SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) {
        hAlign = horizontal;
        vAlign = vertical;
    }

    void EnableBlinking(bool enabled) {
        useBlink = enabled;
    }

    void Update(double deltaTime) {
        elapsedTime += deltaTime;
    }

    void Draw(HDC hdc) {
        if (!transform) return;

        if (useBlink) {
            float blink = static_cast<float>(std::abs(std::sin(elapsedTime * 3)));
            fontColor = Gdiplus::Color(static_cast<BYTE>(fontColor.GetA() * blink), fontColor.GetR(), fontColor.GetG(), fontColor.GetB());
        }

        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

        Gdiplus::FontFamily family(fontFamily.c_str());
        Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

        Gdiplus::SolidBrush brush(fontColor);
        Gdiplus::SolidBrush outlineBrush(outlineColor);

        Gdiplus::StringFormat format;
        format.SetAlignment(hAlign);
        format.SetLineAlignment(vAlign);

        Gdiplus::RectF layoutRect(
            transform->position.x,
            transform->position.y,
            static_cast<Gdiplus::REAL>(width),
            static_cast<Gdiplus::REAL>(height)
        );

        if (useOutline) {
            const int offset = 1;
            for (int dx = -offset; dx <= offset; ++dx) {
                for (int dy = -offset; dy <= offset; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    Gdiplus::RectF offsetRect = layoutRect;
                    offsetRect.X += dx;
                    offsetRect.Y += dy;
                    graphics.DrawString(text.c_str(), -1, &font, offsetRect, &format, &outlineBrush);
                }
            }
        }

        graphics.DrawString(text.c_str(), -1, &font, layoutRect, &format, &brush);
    }
};