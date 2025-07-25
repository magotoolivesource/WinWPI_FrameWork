﻿#pragma once
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

class DebugRectLineComponent : public DebugObject, Transform {

protected:
    bool enabled = true;
    Gdiplus::RectF rect = Gdiplus::RectF(0, 0, 100, 100);
    Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0);
    float thickness = 1.f;

	bool m_ISCameraMatrix = false;
    bool m_ISTransformMatrix = false; // transform 사용하지 않음

public:
    // Getter/Setter for rect
    const Gdiplus::RectF& GetRect() const { return rect; }
    void SetRect(const Gdiplus::RectF& r) { rect = r; }
    void SetRect(Vec2& p_min, Vec2& p_max);
    void SetRect(float p_x, float p_y, float p_width, float p_height);

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
    DebugRectLineComponent() = default;
    DebugRectLineComponent(const Gdiplus::RectF& r, Gdiplus::Color c = Gdiplus::Color(255, 255, 0, 0), float thick = 1.0f)
        : rect(r)
        , color(c)
        , thickness(thick) { }
    virtual ~DebugRectLineComponent() = default;

    //void SetRect(const Gdiplus::RectF& r) { rect = r; }
    //void SetColor(Gdiplus::Color c) { color = c; }
    //void SetThickness(float t) { thickness = t; }


    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;

};