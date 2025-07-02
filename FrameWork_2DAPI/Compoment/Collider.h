#pragma once
#include "../Core/Rect.h"
#include "Component.h"
#include "Transform.h"


class Collider : public Component 
{

public:
    bool Intersects(Collider* other);
    Rect GetWorldRect();
};