#pragma once
#include "Component.h"
#include "../Core/Vector.h"

class Transform : public Component
{
    ////float x = 0, y = 0;
    ////float rotation = 0;
    ////float scaleX = 1, scaleY = 1;

    //float width = 50.0f;
    //float height = 50.0f;
    //float rotation = 0.0f; // in degrees

public:
    Vec2 position;
    float width = 50.0f;
    float height = 50.0f;
    float rotation = 0.0f; // in degrees
};

