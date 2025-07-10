//#pragma once
#include "Collider.h"
#include "../Core/GameObject.h"
#include "Component.h"
#include "Transform.h"


bool Collider::Intersects(Collider* other) {
    Rect a = GetWorldRect();
    Rect b = other->GetWorldRect();
    return a.Intersects(b);
}

Rect Collider::GetWorldRect() {
    Transform* t = owner->GetComponent<Transform>();
    return Rect { t->localposition, t->width, t->height };
}
