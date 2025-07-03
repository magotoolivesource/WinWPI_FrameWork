#pragma once
#include "../Compoment/Component.h"
#include "../Compoment/Transform.h"
#include <functional>


class Button;

class Button : public Component {
public:
    //Button() : onClick(nullptr) {}
    Button() = default;
    Button(const std::function<void(Button*)>& onClick)
        : onClick(onClick) 
    {
    }
    ~Button() = default;

public:
    std::function<void(Button* )> onClick;
    bool isHovered = false;
    bool isPressed = false;

    virtual void Update(float dt) override;

    virtual void Render(HDC hdc) override;
};