#pragma once
#include "../Core/Rect.h"
#include "Component.h"
#include "Transform.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include "../Core/Vector.h"


class Transform;

class LineComponent : public Component {

protected:
    Gdiplus::PointF start = { 0, 0 };
    Gdiplus::PointF end = { 0, 0 };
    Gdiplus::Color color = Gdiplus::Color(255, 0, 255, 0);
    float thickness = 1.f;
    bool m_ISCameraMatrix = false;
    bool m_ISTransformMatrix = false; // transform 사용하지 않음


public:
    Gdiplus::Color GetColor() const { return color; }
    void SetColor(Gdiplus::Color c) { color = c; }

    float GetThickness() const { return thickness; }
    void SetThickness(float t) { thickness = t; }

    bool GetIsCameraMatrix() const { return m_ISCameraMatrix; }
    void SetIsCameraMatrix(bool v) { m_ISCameraMatrix = v; }

    bool GetIsTransformMatrix() const { return m_ISTransformMatrix; }
    void SetIsTransformMatrix(bool v) { m_ISTransformMatrix = v; }

public:
    LineComponent(const Gdiplus::PointF& s, const Gdiplus::PointF& e,
        Gdiplus::Color c = Gdiplus::Color(255, 0, 255, 0), float thick = 1.0f)
        : start(s)
        , end(e)
        , color(c)
        , thickness(thick) 
	{
	}

	virtual ~LineComponent() = default;

    void SetPoints(const Gdiplus::PointF& s, const Gdiplus::PointF& e) {
        start = s;
        end = e;
    }

    //void SetColor(Gdiplus::Color c) { color = c; }
    //void SetThickness(float t) { thickness = t; }


	virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;
};