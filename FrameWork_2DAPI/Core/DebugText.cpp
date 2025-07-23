#include "DebugText.h"
#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"
#include "../Core/MyMath.h"



void DebugText::Initlize() {

	family = new Gdiplus::FontFamily(L"Arial");
    font = new Gdiplus::Font(family, FontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
}

void DebugText::Start() {

}

void DebugText::Update(float dt) {

}

void DebugText::Render(HDC hdc) {
    if (enabled == false) 
		return;

  //  if (!transform) 
		//return;

	if (active == false) 
		return;


    Gdiplus::Graphics graphics(hdc);

	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
    if (m_ISTransformMatrix && mainCamera)
	{
        mainCamera->Begin_Update_GraphicsMatrix(graphics, transform, m_ISCameraMatrix);
	}
	else
	{
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.ResetTransform();
	}
    

	Gdiplus::SolidBrush brush(color);
    //Gdiplus::Pen pen(color, FontSize);
	//graphics.DrawRectangle(&pen, rect);

	Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentNear;
    Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;

	Gdiplus::StringFormat format;
    format.SetAlignment(hAlign);
    format.SetLineAlignment(vAlign);
    Gdiplus::PointF point(Position.x, Position.y);

	graphics.DrawString(m_Text.c_str(), -1, font, point, &brush);
    //graphics.DrawString(drawText.c_str(), -1, &font, point, &brush);



	//Gdiplus::RectF layoutRect(Position.x
	//	, Position.y
	//	, static_cast<Gdiplus::REAL>(300), static_cast<Gdiplus::REAL>(150));
 //   Gdiplus::Status state = graphics.DrawString(m_Text.c_str(), -1, font, layoutRect, &format, &brush);


	if (m_ISTransformMatrix && mainCamera)
	{
        mainCamera->End_Update_GraphicsMatrix(graphics, transform);
	}
	else
	{
        graphics.ResetTransform();
	}

}
