#pragma once
#include <cmath>

struct Vec2 {
    float x, y;

    Vec2 Rotate(float angleDegrees) const {
        float rad = angleDegrees * 3.14159265f / 180.0f;
        float cosA = cosf(rad);
        float sinA = sinf(rad);
        return {
            x * cosA - y * sinA,
            x * sinA + y * cosA
        };
    }
};