#include "UI_SelectBTNCom.h"
#include "Core/InputManager.h"
#include <cmath>
#include <algorithm>
#include <Core/GameObject.h>
#include <Compoment/Camera.h>
#include <Manager/CameraManager.h>




using namespace Gdiplus;
using namespace std;

UI_SelectBTNCom::~UI_SelectBTNCom( )
{

}

void UI_SelectBTNCom::Start()
{
	Button::Start();

}

void UI_SelectBTNCom::Update(float dt)
{
	Button::Update(dt);


}

void UI_SelectBTNCom::Render(HDC hdc)
{
	__super::Render(hdc);



	ButtonRender(hdc);

	

}

// https://chatgpt.com/c/689982c0-33dc-832d-95d2-0863e3956080
void UI_SelectBTNCom::ButtonRender(HDC hdc)
{



	if ( !Button::enabled ) return;

	Transform* temptransform = Button::transform;
	if ( !m_IconImg || !temptransform ) return;


	// --------------------- 색상 지정
	// 색상 값 지정
	Color tintcolor = m_DefaultColor;
	if ( isPressed )
		tintcolor = m_PressColor;
	else if ( isHovered )
		tintcolor = m_HoverColor;


	// 색상 비율(0~1 범위)
	float r = tintcolor.GetR( ) / 255.0f;
	float gC = tintcolor.GetG( ) / 255.0f;
	float b = tintcolor.GetB( ) / 255.0f;

	// ColorMatrix 생성
	ColorMatrix colorMatrix =
	{
		r,     0.0f,  0.0f,  0.0f, 0.0f,
		0.0f,  gC,    0.0f,  0.0f, 0.0f,
		0.0f,  0.0f,  b,     0.0f, 0.0f,
		0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.0f,  0.0f,  0.0f,  0.0f, 1.0f
	};

	ImageAttributes imgAttr;
	imgAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	// --------------------- 색상 지정



	// --------------------- 화면 그리기
	Gdiplus::Graphics graphics(hdc);
	Camera* mainCamera = CameraManager::GetI( )->GetMainCamera( );
	mainCamera->Begin_Update_GraphicsMatrix(graphics, temptransform);

	ImageComponent* imagecom = m_IconImg;
	Image* image = imagecom->GetImage( );
	Gdiplus::Rect srcRect;
	Gdiplus::Rect destRect;
	bool isdrawrect = false;
	imagecom->GetDrawSRCNDESTRect(&srcRect, &destRect, &isdrawrect);

	if ( isdrawrect )
	{
		graphics.DrawImage(image, srcRect, destRect.X, destRect.Y, destRect.Width, destRect.Height
			, Gdiplus::UnitPixel
			, &imgAttr);
	}
	else
	{
		Gdiplus::Rect innerdestRect(0, 0, srcRect.Width, srcRect.Height);
		graphics.DrawImage(image, srcRect
			, innerdestRect.X, innerdestRect.Y, innerdestRect.Width, innerdestRect.Height
			, Gdiplus::UnitPixel
			, &imgAttr);
	}
	// --------------------- 화면 그리기

	mainCamera->End_Update_GraphicsMatrix(graphics, temptransform);

}

void UI_SelectBTNCom::Initialize( )
{
	__super::Initialize( );

	//m_DefaultColor.SetValue(Gdiplus::Color::MakeARGB(255, 255, 255, 255));
	m_DefaultColor = Gdiplus::Color::MakeARGB(255, 255, 255, 255);

	m_IconImg = this->owner->AddComponent<ImageComponent>( );
	m_IconImg->transform->SetEnabled(false);
	
	
}

void UI_SelectBTNCom::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	Initialize( );
}

Gdiplus::Image* UI_SelectBTNCom::GetImage( )
{
	if ( m_IconImg )
		return m_IconImg->GetImage( );
	return nullptr;
}

void UI_SelectBTNCom::ImageLoadImage(std::wstring p_loadimg)
{
	m_IconImg->ImageLoadImage(p_loadimg);
}

void UI_SelectBTNCom::SetDrawRect(int p_drawwidth, int p_drawheight
	, int p_imgleft, int p_imgtop, int p_imgright, int p_imgbottom)
{
	m_IconImg->SetDrawRect(p_drawwidth, p_drawheight
		, p_imgleft, p_imgtop, p_imgright, p_imgbottom);
}
