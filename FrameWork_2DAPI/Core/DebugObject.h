#pragma once
#include "Rect.h"
#include "GameObject.h"
#include "../Compoment/Component.h"
#include "../Compoment/Transform.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include "Vector.h"


class Transform;

class DebugObject {
protected:
	bool active = true;

public:
    DebugObject() { }
    virtual ~DebugObject() = default;

    //void SetRect(const Gdiplus::RectF& r) { rect = r; }
    //void SetColor(Gdiplus::Color c) { color = c; }
    //void SetThickness(float t) { thickness = t; }

	void SetActive(bool a) { active = a; }
    bool IsActive() const { return active; }

    virtual void Start() { };
    virtual void Update(float dt) { };
    virtual void Render(HDC hdc) { };
};