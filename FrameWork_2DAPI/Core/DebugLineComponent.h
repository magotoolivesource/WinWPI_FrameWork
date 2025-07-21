#pragma once
#include "Rect.h"
#include "DebugObject.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include "Vector.h"

#include "../Compoment/Transform.h"

//class Transform;

class DebugLineComponent : public DebugObject, Transform {

protected:
    bool enabled = true;
    Gdiplus::RectF rect = Gdiplus::RectF(0, 0, 100, 100);

	Vec2 startPoint = Vec2(0, 0);
    Vec2 endPoint = Vec2(0, 0);
    Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0);
    float thickness = 1.f;

	bool m_ISCameraMatrix = false;
    bool m_ISTransformMatrix = false; // transform 사용하지 않음

public:
	void SetLine(const Vec2& p_start, const Vec2& p_end)
	{
		startPoint = p_start;
        endPoint = p_end;
	}
	void GetLine(OUT Vec2& p_start, OUT Vec2& p_end) const {
		p_start = startPoint;
		p_end = endPoint;
    }


    // Getter/Setter for color
    Gdiplus::Color GetColor() const { return color; }
    void SetColor(Gdiplus::Color c) { color = c; }

    // Getter/Setter for thickness
    float GetThickness() const { return thickness; }
    void SetThickness(float t) { thickness = t; }

    // Getter/Setter for m_ISCameraMatrix
    bool GetIsCameraMatrix() const { return m_ISCameraMatrix; }
    void SetIsCameraMatrix(bool v) { m_ISCameraMatrix = v; }

    // Getter/Setter for m_ISTransformMatrix
    bool GetIsTransformMatrix() const { return m_ISTransformMatrix; }
    void SetIsTransformMatrix(bool v) { m_ISTransformMatrix = v; }

public:
    DebugLineComponent() = default;
    DebugLineComponent(const Vec2& p_start
		, const Vec2& p_end
		, Gdiplus::Color c = Gdiplus::Color(255, 255, 0, 0)
		, float thick = 1.0f 
	)
        : startPoint(p_start)
        , endPoint(p_end)
        , color(c)
        , thickness(thick) { }
    virtual ~DebugLineComponent() = default;

    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;

};