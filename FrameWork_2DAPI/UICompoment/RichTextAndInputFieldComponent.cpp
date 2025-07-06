#pragma once
#include "RichTextAndInputFieldComponent.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Core/DefineHeader.h"









void InputFieldComponent::Draw(HDC hdc) {
	if ( !transform ) return;
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0));
	std::wstring drawText = text.empty( ) ? placeholder : text;
	Gdiplus::PointF point(transform->position.x, transform->position.y);
	graphics.DrawString(drawText.c_str( ), -1, &font, point, &brush);
}