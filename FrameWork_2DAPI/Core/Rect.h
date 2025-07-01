#pragma once
#include "Vector.h"

struct Rect {
    Vec2 pos;
    float width, height;

    bool Intersects(const Rect& other) {
        return !(pos.x + width < other.pos.x || pos.x > other.pos.x + other.width ||
                 pos.y + height < other.pos.y || pos.y > other.pos.y + other.height);
    }
};