#pragma once
#include "Vector.h"
#include "Rect.h"
#include "../Compoment/Component.h"
//#include "Transform.h"
#include <windows.h>
#include <gdiplus.h>


using namespace Gdiplus;

class Viewport {
private:
    Gdiplus::RectF viewRect;

public:
    void SetViewRect(float x, float y, float width, float height) { viewRect = Gdiplus::RectF(x, y, width, height); }

    bool IsVisible(const Vec2& screenPos, float width, float height) const {
        Gdiplus::RectF objectRect(screenPos.x, screenPos.y, width, height);
        return viewRect.IntersectsWith(objectRect);
    }

    const Gdiplus::RectF& GetViewRect() const { return viewRect; }
};
