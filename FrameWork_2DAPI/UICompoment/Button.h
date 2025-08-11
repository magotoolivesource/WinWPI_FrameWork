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


protected:
    Gdiplus::Color m_DefaultColor = Gdiplus::Color(255, 0, 0, 240);
    Gdiplus::Color m_HoverColor = Gdiplus::Color(255, 220, 0, 0);
    Gdiplus::Color m_PressColor = Gdiplus::Color(255, 0, 200, 200);

private:
    Gdiplus::Matrix m_TempTransMatrix;
    Gdiplus::REAL matrixElements[6];

	bool ISClick();

public:
    std::function<void(Button* )> onClick;
    bool isHovered = false;
    bool isPressed = false;

    virtual void Update(float dt) override;

    virtual void Render(HDC hdc) override;

public:
	void OnClickCallBack(const std::function<void(Button*)>& p_onClick);

protected:
	float width = 50.0f;
	float height = 50.0f;
public:
	float getWidth( ) const { return width; }
	float getHeight( ) const { return height; }
	void setWidth(float w) { width = w; }
	void setHeight(float h) { height = h; }

	void SetBTNSize(float p_w, float p_h) { width = p_w; height = p_h; }

};