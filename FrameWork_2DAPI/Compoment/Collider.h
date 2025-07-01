#pragma once
#include "../Core/Rect.h"
#include "Component.h"
#include "Transform.h"
class Collider : public Component 
{

public:
    bool Intersects(Collider* other) {
        Rect a = GetWorldRect();
        Rect b = other->GetWorldRect();
        return a.Intersects(b);
    }

    Rect GetWorldRect() 
    {
        Transform* t = owner->GetComponent<Transform>();
        return Rect{ t->position, t->width, t->height };
    }
};