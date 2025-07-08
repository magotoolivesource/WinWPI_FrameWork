#pragma once
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Core/InputManager.h"
#include "Button.h"

void Button::Update(float dt)
{
    Transform* t = owner->GetComponent<Transform>();
    if (!t) return;

	Vec2 wpos = t->GetWorldPosition( );

    POINT mouse = InputManager::mousePosition;
    bool inside = mouse.x >= wpos.x && mouse.x <= wpos.x + t->width &&
        mouse.y >= wpos.y && mouse.y <= wpos.y + t->height;

    if (inside) {
        isHovered = true;
        if (InputManager::mouseLeftDown && !isPressed) {
            isPressed = true;
        }
        else if (!InputManager::mouseLeftDown && isPressed) {
            isPressed = false;
            if (onClick) onClick(this);
        }
    }
    else {
        isHovered = false;
        isPressed = false;
    }
}

void Button::Render(HDC hdc)
{
    Transform* t = owner->GetComponent<Transform>();
    if (!t) return;


	Vec2 wpos = t->GetWorldPosition( );

    HBRUSH brush;
    if (isPressed) brush = CreateSolidBrush(RGB(200, 200, 200));
    else if (isHovered) brush = CreateSolidBrush(RGB(220, 220, 220));
    else brush = CreateSolidBrush(RGB(240, 240, 240));

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Rectangle(hdc, (int) wpos.x, (int) wpos.y, (int)( wpos.x + t->width), (int)( wpos.y + t->height));
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}