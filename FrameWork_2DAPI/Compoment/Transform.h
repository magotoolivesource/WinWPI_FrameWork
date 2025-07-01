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

public:
    // Getters
    Vec2 getPosition() const { return position; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getRotation() const { return rotation; }
    // Setters
    void setPosition(const Vec2& pos) { position = pos; }
    void setPosition(int p_x, int p_y) 
    { 
        position.x = p_x;
        position.y = p_y;
    }
    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
    void setRotation(float rot) { rotation = rot; }
    // Override the clone method
	//virtual Transform* clone() const override { return new Transform(*this); }
};

