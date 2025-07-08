#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
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

    void Draw(HDC hdc) {
        if (!transform) return;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

        Gdiplus::FontFamily family(fontFamily.c_str());
        Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

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
                Gdiplus::SolidBrush brush(seg.color);
                graphics.DrawString(line.c_str(), -1, &font, layoutRect, &format, &brush);

                y += lineHeight;
            }
        }
    }
};