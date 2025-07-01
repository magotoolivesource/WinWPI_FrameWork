#pragma once
#include "../Compoment/Component.h"
#include "../Compoment/Transform.h"
#include <functional>

class Button : public Component {
public:
    std::function<void()> onClick;
    bool isHovered = false;
    bool isPressed = false;

    void Update(float dt) override;

    void Render(HDC hdc) override;
};