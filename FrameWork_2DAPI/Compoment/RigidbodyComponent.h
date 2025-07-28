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

class RigidbodyComponent : public Component {

private:
	Gdiplus::RectF rect = Gdiplus::RectF(0, 0, 100, 100);
	Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0);
	float thickness = 1.f;

	bool m_ISCameraMatrix = false;
	bool m_ISTransformMatrix = false; // transform 사용하지 않음

public:
	RigidbodyComponent(const Gdiplus::RectF& r, Gdiplus::Color c = Gdiplus::Color(255, 255, 0, 0), float thick = 1.0f)
        : rect(r)
        , color(c)
        , thickness(thick) { }
    virtual ~RigidbodyComponent() = default;


    virtual void Start() override;
    virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;
};