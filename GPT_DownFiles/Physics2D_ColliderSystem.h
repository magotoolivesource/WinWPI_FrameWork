#pragma once
#include <gdiplus.h>
#include <vector>
#include <cmath>

class Collider;

struct RaycastHit {
    Collider* collider = nullptr;
    Gdiplus::PointF point = {0, 0};
    float distance = 0;
};

class Collider {
public:
    Gdiplus::PointF position;
    float width = 0;
    float height = 0;

    Collider(Gdiplus::PointF pos, float w, float h)
        : position(pos), width(w), height(h) {}

    Gdiplus::RectF GetBounds() const {
        return Gdiplus::RectF(position.X, position.Y, width, height);
    }

    bool OverlapsPoint(const Gdiplus::PointF& point) const {
        return GetBounds().Contains(point);
    }

    bool OverlapsBox(const Gdiplus::RectF& box) const {
        return GetBounds().IntersectsWith(box);
    }

    bool OverlapsCircle(const Gdiplus::PointF& center, float radius) const {
        Gdiplus::RectF rect = GetBounds();
        float closestX = std::max(rect.X, std::min(center.X, rect.X + rect.Width));
        float closestY = std::max(rect.Y, std::min(center.Y, rect.Y + rect.Height));
        float dx = center.X - closestX;
        float dy = center.Y - closestY;
        return (dx * dx + dy * dy) <= radius * radius;
    }

    bool OverlapsCapsule(const Gdiplus::PointF& center, float radius, float height) const {
        Gdiplus::RectF capsule(center.X - radius, center.Y - height / 2, radius * 2, height);
        return GetBounds().IntersectsWith(capsule);
    }

    bool Raycast(const Gdiplus::PointF& origin, const Gdiplus::PointF& dir, float maxDist, RaycastHit& hitInfo) const {
        Gdiplus::RectF bounds = GetBounds();

        float tmin = (bounds.X - origin.X) / dir.X;
        float tmax = ((bounds.X + bounds.Width) - origin.X) / dir.X;

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (bounds.Y - origin.Y) / dir.Y;
        float tymax = ((bounds.Y + bounds.Height) - origin.Y) / dir.Y;

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        float t = std::max(tmin, tymin);
        if (t < 0 || t > maxDist) return false;

        hitInfo.collider = const_cast<Collider*>(this);
        hitInfo.point = {origin.X + dir.X * t, origin.Y + dir.Y * t};
        hitInfo.distance = t;
        return true;
    }
};

class Physics2D {
public:
    static std::vector<Collider*> colliders;

    static void Register(Collider* c) {
        colliders.push_back(c);
    }

    static void Clear() {
        colliders.clear();
    }

    static bool Raycast(const Gdiplus::PointF& origin, const Gdiplus::PointF& dir, float maxDist, RaycastHit& hitInfo) {
        bool hit = false;
        float minDist = maxDist;
        for (auto* c : colliders) {
            RaycastHit tempHit;
            if (c->Raycast(origin, dir, maxDist, tempHit)) {
                if (tempHit.distance < minDist) {
                    hitInfo = tempHit;
                    minDist = tempHit.distance;
                    hit = true;
                }
            }
        }
        return hit;
    }

    static std::vector<RaycastHit> RaycastAll(const Gdiplus::PointF& origin, const Gdiplus::PointF& dir, float maxDist) {
        std::vector<RaycastHit> hits;
        for (auto* c : colliders) {
            RaycastHit hitInfo;
            if (c->Raycast(origin, dir, maxDist, hitInfo)) {
                hits.push_back(hitInfo);
            }
        }
        return hits;
    }

    static std::vector<Collider*> OverlapBox(const Gdiplus::RectF& box) {
        std::vector<Collider*> result;
        for (auto* c : colliders) {
            if (c->OverlapsBox(box)) result.push_back(c);
        }
        return result;
    }

    static std::vector<Collider*> OverlapCircle(const Gdiplus::PointF& center, float radius) {
        std::vector<Collider*> result;
        for (auto* c : colliders) {
            if (c->OverlapsCircle(center, radius)) result.push_back(c);
        }
        return result;
    }

    static std::vector<Collider*> OverlapCapsule(const Gdiplus::PointF& center, float radius, float height) {
        std::vector<Collider*> result;
        for (auto* c : colliders) {
            if (c->OverlapsCapsule(center, radius, height)) result.push_back(c);
        }
        return result;
    }

    static std::vector<Collider*> OverlapPoint(const Gdiplus::PointF& point) {
        std::vector<Collider*> result;
        for (auto* c : colliders) {
            if (c->OverlapsPoint(point)) result.push_back(c);
        }
        return result;
    }
};

std::vector<Collider*> Physics2D::colliders;