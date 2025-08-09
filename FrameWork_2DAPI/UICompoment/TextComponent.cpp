#include "TextComponent.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Core/DefineHeader.h"
#include "../Core/InputManager.h"

#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"



TextComponent::TextComponent()
{
    //Initialize();
    
}

TextComponent::~TextComponent()
{
    transform = nullptr;
}

void TextComponent::Initialize_AddCompoment()
{
    transform = this->owner->GetComponent<Transform>();

    //Gdiplus::FontFamily family(fontFamily.c_str());
    //Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    family = new Gdiplus::FontFamily(fontFamily.c_str());
    font = new Gdiplus::Font(family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

}

void TextComponent::Initialize()
{
    
}

void TextComponent::Release()
{
    SAFEDELETE(font);
    SAFEDELETE(family);

}

void TextComponent::SetTransform(Transform* t) {
    transform = t;
}

void TextComponent::SetText(const std::wstring& str) {
    text = str;
}

void TextComponent::SetFont(const std::wstring& family, float size) {
    fontFamily = family;
    fontSize = size;
}

void TextComponent::SetFontColor(BYTE a, BYTE r, BYTE g, BYTE b) {
    fontColor = Gdiplus::Color(a, r, g, b);
}

void TextComponent::SetOutlineColor(BYTE a, BYTE r, BYTE g, BYTE b) {
    outlineColor = Gdiplus::Color(a, r, g, b);
    useOutline = true;
}

void TextComponent::SetSize(int w, int h) {
    width = w;
    height = h;
}

void TextComponent::SetAlignment(Gdiplus::StringAlignment horizontal, Gdiplus::StringAlignment vertical) {
    hAlign = horizontal;
    vAlign = vertical;
}

void TextComponent::EnableBlinking(bool enabled) {
    useBlink = enabled;
}

void TextComponent::Update(float dt) {
    //elapsedTime += dt;

}

void TextComponent::Render(HDC hdc) {
    if (!transform) return;

    if (useBlink) {
        float blink = static_cast<float>(std::abs(std::sin(elapsedTime * 3)));
        fontColor = Gdiplus::Color(static_cast<BYTE>(fontColor.GetA() * blink), fontColor.GetR(), fontColor.GetG(), fontColor.GetB());
    }

    Gdiplus::Graphics graphics(hdc);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);


	if ( m_ISCameraMatrix )
	{
        Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
        Gdiplus::Matrix& campmat = mainCamera->GetCameraWorldMatrix();
        graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);
	}
	else
	{
        graphics.ResetTransform();
	}


    //Gdiplus::FontFamily family(fontFamily.c_str());
    //Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush brush(fontColor);
    Gdiplus::SolidBrush outlineBrush(outlineColor);

    Gdiplus::StringFormat format;
    format.SetAlignment(hAlign);
    format.SetLineAlignment(vAlign);

	Vec2 localpos = transform->getLocalPosition( );
    Gdiplus::RectF layoutRect(
		localpos.x,
		localpos.y,
        static_cast<Gdiplus::REAL>(width),
        static_cast<Gdiplus::REAL>(height)
    );

    if (useOutline) {
        const int offset = 1;
        for (int dx = -offset; dx <= offset; ++dx) {
            for (int dy = -offset; dy <= offset; ++dy) {
                if (dx == 0 && dy == 0) continue;
                Gdiplus::RectF offsetRect = layoutRect;
                offsetRect.X += dx;
                offsetRect.Y += dy;
                graphics.DrawString(text.c_str(), -1, font, offsetRect, &format, &outlineBrush);
            }
        }
    }

    graphics.DrawString(text.c_str(), -1, font, layoutRect, &format, &brush);


	if (m_ISCameraMatrix) {
        graphics.ResetTransform();
    }

}