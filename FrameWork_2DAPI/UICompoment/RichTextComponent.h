#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include "../Compoment/Component.h"
//#include "../Compoment/Transform.h"
//#pragma comment(lib, "gdiplus.lib")


// 한글 사용시 소스파일 인코딩은 UTF-8-BOM 으로 설정 해야지 한글 안깨짐
class Transform;

class RichTextComponent : public Component {

public:
	RichTextComponent() = default;
	virtual ~RichTextComponent( );
	//// Disable copy and move semantics
	//RichTextComponent(const RichTextComponent&) = delete;
	//RichTextComponent& operator=(const RichTextComponent&) = delete;
	//RichTextComponent(RichTextComponent&&) = delete;
	//RichTextComponent& operator=(RichTextComponent&&) = delete;

public:
	virtual void Initialize( ) override;
	virtual void Initialize_AddCompoment( ) override;
	virtual void Release( ) override;


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

	Gdiplus::FontFamily* family;
	Gdiplus::Font* font;

    struct TextSegment {
        std::wstring text;
        Gdiplus::Color color;
    };

    std::vector<TextSegment> parsedSegments;

	Gdiplus::Color ParseColor(const std::wstring& hex);

	void ParseRichText( );

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
		if ( bold ) fontStyle |= Gdiplus::FontStyleBold;
		if ( italic ) fontStyle |= Gdiplus::FontStyleItalic;
		if ( underline ) fontStyle |= Gdiplus::FontStyleUnderline;
	}
	void SetOutline(bool use, BYTE a, BYTE r, BYTE g, BYTE b) {
		useOutline = use;
		outlineColor = Gdiplus::Color(a, r, g, b);
	}


	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;

	void Draw(HDC hdc);
};