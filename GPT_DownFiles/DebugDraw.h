#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Physics2D_ColliderSystem.h"

class DebugDraw {
private:
    static Gdiplus::Pen colliderPen;
    static Gdiplus::SolidBrush pointBrush;

public:
    static void Init() {
        colliderPen.SetColor(Gdiplus::Color(128, 255, 0, 0));  // semi-transparent red
        pointBrush.SetColor(Gdiplus::Color(200, 0, 255, 0));   // semi-transparent green
    }

    static void DrawAllColliders(Gdiplus::Graphics& g) {
        for (auto* collider : Physics2D::colliders) {
            auto bounds = collider->GetBounds();
            g.DrawRectangle(&colliderPen, bounds);
        }
    }

    static void DrawRaycastHit(Gdiplus::Graphics& g, const RaycastHit& hit) {
        if (hit.collider) {
            float size = 4.0f;
            g.FillEllipse(&pointBrush, hit.point.X - size/2, hit.point.Y - size/2, size, size);
        }
    }

    static void DrawRay(Gdiplus::Graphics& g, const Gdiplus::PointF& origin, const Gdiplus::PointF& dir, float length) {
        Gdiplus::Pen rayPen(Gdiplus::Color(180, 0, 0, 255), 1.5f); // semi-transparent blue
        Gdiplus::PointF end = { origin.X + dir.X * length, origin.Y + dir.Y * length };
        g.DrawLine(&rayPen, origin, end);
    }
};

// Initialize static members
Gdiplus::Pen DebugDraw::colliderPen(Gdiplus::Color(128, 255, 0, 0), 1.0f);
Gdiplus::SolidBrush DebugDraw::pointBrush(Gdiplus::Color(200, 0, 255, 0));