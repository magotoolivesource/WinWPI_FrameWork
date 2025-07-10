#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <cmath>
#include "../Compoment/Component.h"
//#pragma comment(lib, "gdiplus.lib")

class Transform;

// 한글 사용시 소스파일 인코딩은 UTF-8-BOM 으로 설정 해야지 한글 안깨짐
class TextComponent : public Component {
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


    Gdiplus::FontFamily* family;
    Gdiplus::Font* font;

public:
    TextComponent();
    virtual ~TextComponent();

public:
    virtual void Initialize() override;
    virtual void Initialize_AddCompoment() override;
    virtual void Release() override;

public:
    void SetTransform(Transform* t);

    void SetText(const std::wstring& str);

    void SetFont(const std::wstring& family, float size);

    void SetFontColor(BYTE a, BYTE r, BYTE g, BYTE b);

    void SetOutlineColor(BYTE a, BYTE r, BYTE g, BYTE b);

    void SetSize(int w, int h);

    void SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical);

    void EnableBlinking(bool enabled);

    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;
};
