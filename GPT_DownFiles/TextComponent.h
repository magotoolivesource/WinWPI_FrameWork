#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "Transform.h"

#pragma comment(lib, "gdiplus.lib")

class TextComponent {
private:
    Transform* transform = nullptr;
    std::wstring text = L"";
    std::wstring fontFamily = L"Arial";
    float fontSize = 24.0f;
    Gdiplus::Color fontColor = Gdiplus::Color(255, 0, 0, 0); // default black
    int width = 200;
    int height = 50;
    Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentNear;   // Left
    Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;   // Top

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

    void SetSize(int w, int h) {
        width = w;
        height = h;
    }

    void SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) {
        hAlign = horizontal;
        vAlign = vertical;
    }

    void Draw(HDC hdc) {
        if (!transform) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

        Gdiplus::FontFamily family(fontFamily.c_str());
        Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

        Gdiplus::SolidBrush brush(fontColor);

        Gdiplus::StringFormat format;
        format.SetAlignment(hAlign);      // Left, Center, Right
        format.SetLineAlignment(vAlign);  // Top, Center, Bottom

        Gdiplus::RectF layoutRect(
            transform->position.x,
            transform->position.y,
            static_cast<Gdiplus::REAL>(width),
            static_cast<Gdiplus::REAL>(height)
        );

        graphics.DrawString(text.c_str(), -1, &font, layoutRect, &format, &brush);
    }
};