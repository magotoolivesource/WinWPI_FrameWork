#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Transform.h"

class Material {
private:
    Gdiplus::Image* image = nullptr;
    Gdiplus::Rect srcRect;

public:
    Material(const std::wstring& filepath, const Gdiplus::Rect& region = Gdiplus::Rect())
        : image(nullptr), srcRect(region) {
        image = Gdiplus::Image::FromFile(filepath.c_str());
        if (srcRect.Width == 0 || srcRect.Height == 0) {
            srcRect = Gdiplus::Rect(0, 0, image->GetWidth(), image->GetHeight());
        }
    }

    ~Material() {
        if (image) delete image;
    }

    Gdiplus::Image* GetImage() const { return image; }
    const Gdiplus::Rect& GetSourceRect() const { return srcRect; }
};

// --------- Object Group ----------
class ObjectGroup {
private:
    std::vector<class GameObject*> members;
public:
    void Add(GameObject* obj) {
        members.push_back(obj);
    }

    const std::vector<GameObject*>& GetMembers() const {
        return members;
    }
};

// ---------- Camera --------------
class Camera {
private:
    Gdiplus::PointF position = {0, 0};
    float zoom = 1.0f;

public:
    void SetPosition(const Gdiplus::PointF& pos) {
        position = pos;
    }

    void SetZoom(float z) {
        zoom = z;
    }

    Gdiplus::PointF ApplyToPoint(const Gdiplus::PointF& worldPos) const {
        return {
            (worldPos.X - position.X) * zoom,
            (worldPos.Y - position.Y) * zoom
        };
    }

    void ApplyToMatrix(Gdiplus::Matrix& matrix) const {
        matrix.Translate(-position.X, -position.Y);
        matrix.Scale(zoom, zoom);
    }
};