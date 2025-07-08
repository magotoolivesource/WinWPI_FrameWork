#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <functional>
#include <regex>
#include <sstream>
#include "Transform.h"

#pragma comment(lib, "gdiplus.lib")

class RichTextComponent {
private:
    Transform* transform = nullptr;
    std::wstring rawText = L"";
    std::wstring fontFamily = L"Arial";
    float fontSize = 24.0f;
    int width = 400;
    int height = 100;
    Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentCenter;
    Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;
    bool useOutline = false;
    Gdiplus::Color outlineColor = Gdiplus::Color(255, 0, 0, 0);
    int fontStyle = Gdiplus::FontStyleRegular;

    struct TextSegment {
        std::wstring text;
        Gdiplus::Color color;
    };

    std::vector<TextSegment> parsedSegments;

    Gdiplus::Color ParseColor(const std::wstring& hex) {
        unsigned int value = 0;
        std::wstringstream ss;
        ss << std::hex << hex;
        ss >> value;
        BYTE a = (value >> 24) & 0xFF;
        BYTE r = (value >> 16) & 0xFF;
        BYTE g = (value >> 8) & 0xFF;
        BYTE b = value & 0xFF;
        return Gdiplus::Color(a, r, g, b);
    }

    void ParseRichText() {
        parsedSegments.clear();
        std::wregex tag(L"<color=#([0-9A-Fa-f]{8})>(.*?)</color>");
        std::wsregex_iterator iter(rawText.begin(), rawText.end(), tag);
        std::wsregex_iterator end;

        size_t lastPos = 0;
        for (; iter != end; ++iter) {
            size_t matchPos = iter->position();
            if (matchPos > lastPos) {
                parsedSegments.push_back({
                    rawText.substr(lastPos, matchPos - lastPos),
                    Gdiplus::Color(255, 0, 0, 0)
                });
            }
            parsedSegments.push_back({
                (*iter)[2].str(),
                ParseColor((*iter)[1].str())
            });
            lastPos = matchPos + iter->length();
        }

        if (lastPos < rawText.size()) {
            parsedSegments.push_back({
                rawText.substr(lastPos),
                Gdiplus::Color(255, 0, 0, 0)
            });
        }
    }

public:
    void SetTransform(Transform* t) { transform = t; }
    void SetText(const std::wstring& text) {
        rawText = text;
        ParseRichText();
    }
    void SetFont(const std::wstring& family, float size) {
        fontFamily = family;
        fontSize = size;
    }
    void SetSize(int w, int h) {
        width = w;
        height = h;
    }
    void SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) {
        hAlign = horizontal;
        vAlign = vertical;
    }
    void SetStyle(bool bold, bool italic, bool underline) {
        fontStyle = Gdiplus::FontStyleRegular;
        if (bold) fontStyle |= Gdiplus::FontStyleBold;
        if (italic) fontStyle |= Gdiplus::FontStyleItalic;
        if (underline) fontStyle |= Gdiplus::FontStyleUnderline;
    }
    void SetOutline(bool use, BYTE a, BYTE r, BYTE g, BYTE b) {
        useOutline = use;
        outlineColor = Gdiplus::Color(a, r, g, b);
    }

    void Draw(HDC hdc) {
        if (!transform) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
        Gdiplus::FontFamily family(fontFamily.c_str());
        Gdiplus::Font font(&family, fontSize, fontStyle, Gdiplus::UnitPixel);

        Gdiplus::StringFormat format;
        format.SetAlignment(hAlign);
        format.SetLineAlignment(vAlign);
        format.SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);

        float lineHeight = fontSize + 4.0f;
        float y = transform->position.y;

        for (const auto& seg : parsedSegments) {
            std::wistringstream stream(seg.text);
            std::wstring line;
            while (std::getline(stream, line, L'\n')) {
                if (y > transform->position.y + height) break;
                Gdiplus::RectF layoutRect(transform->position.x, y, (Gdiplus::REAL)width, (Gdiplus::REAL)lineHeight);
                if (useOutline) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            if (dx == 0 && dy == 0) continue;
                            Gdiplus::RectF outlineRect = layoutRect;
                            outlineRect.X += dx;
                            outlineRect.Y += dy;
                            Gdiplus::SolidBrush outlineBrush(outlineColor);
                            graphics.DrawString(line.c_str(), -1, &font, outlineRect, &format, &outlineBrush);
                        }
                    }
                }
                Gdiplus::SolidBrush brush(seg.color);
                graphics.DrawString(line.c_str(), -1, &font, layoutRect, &format, &brush);
                y += lineHeight;
            }
        }
    }
};

class InputFieldComponent {
private:
    Transform* transform = nullptr;
    std::wstring text;
    std::wstring placeholder = L"";
    std::function<void(const std::wstring&)> onChange;
    bool focused = false;

public:
    void SetTransform(Transform* t) { transform = t; }
    void SetPlaceholder(const std::wstring& hint) { placeholder = hint; }
    void SetOnChange(std::function<void(const std::wstring&)> callback) { onChange = callback; }

    void InputChar(wchar_t c) {
        text += c;
        if (onChange) onChange(text);
    }

    void Backspace() {
        if (!text.empty()) {
            text.pop_back();
            if (onChange) onChange(text);
        }
    }

    void SetFocus(bool isFocused) {
        focused = isFocused;
    }

    void Draw(HDC hdc) {
        if (!transform) return;
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::FontFamily fontFamily(L"Arial");
        Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0));
        std::wstring drawText = text.empty() ? placeholder : text;
        Gdiplus::PointF point(transform->position.x, transform->position.y);
        graphics.DrawString(drawText.c_str(), -1, &font, point, &brush);
    }
};