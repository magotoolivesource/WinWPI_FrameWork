#pragma once
#include "Component.h"

class Transform : public Component {
public:
    struct Vector2 {
        float x, y;
        Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    };

    Vector2 position;
    float rotation = 0.0f;

protected:
    Transform() = default;

    friend class GameObject;
};