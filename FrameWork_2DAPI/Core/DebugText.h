#pragma once
#include "Rect.h"
#include "DebugObject.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include <string>
#include "Vector.h"

#include "../Compoment/Transform.h"

//class Transform;

class DebugText : public DebugObject, Transform {

protected:
    bool enabled = true;

	Gdiplus::Color color = Gdiplus::Color(255, 255, 255, 0);
	Vec2 Position = Vec2(0, 0);
    int FontSize = 24;

	bool m_ISCameraMatrix = false;
    bool m_ISTransformMatrix = false; // transform 사용하지 않음

	std::wstring m_TextW = L"";

	Gdiplus::FontFamily* family;
    Gdiplus::Font* font;

public:
    void Initlize();

	// Getter/Setter for m_Text
    const std::wstring& GetTextW() const { return m_TextW; }
    void SetTextW(const std::wstring& text) { m_TextW = text; }

    void SetPosition(const Vec2& p_pos)
	{
        Position = p_pos;
	}
    void GetPosition(OUT Vec2& p_pos, OUT Vec2& p_end) const {
        p_pos = Position;
    }

    // Getter/Setter for color
    Gdiplus::Color GetColor() const { return color; }
    void SetColor(Gdiplus::Color c) { color = c; }

    // Getter/Setter for thickness
    int GetFontSize() const { return FontSize; }
    void SetFontSize(int t) { FontSize = t; }

    // Getter/Setter for m_ISCameraMatrix
    bool GetIsCameraMatrix() const { return m_ISCameraMatrix; }
    void SetIsCameraMatrix(bool v) { m_ISCameraMatrix = v; }

    // Getter/Setter for m_ISTransformMatrix
    bool GetIsTransformMatrix() const { return m_ISTransformMatrix; }
    void SetIsTransformMatrix(bool v) { m_ISTransformMatrix = v; }

public:
    DebugText() { Initlize(); };
    DebugText(const Vec2& p_pos
		, Gdiplus::Color c = Gdiplus::Color(255, 255, 0, 0)
		, int p_fontsize = 18
	): Position(p_pos)
        , color(c)
        , FontSize(p_fontsize) {
        Initlize();
	}
    virtual ~DebugText() = default;

    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;

};