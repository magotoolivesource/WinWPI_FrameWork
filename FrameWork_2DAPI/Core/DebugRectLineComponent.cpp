#include "DebugRectLineComponent.h"
#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"
#include "../Core/MyMath.h"


void DebugRectLineComponent::SetRect(Vec2& p_min, Vec2& p_max) 
{
	MyMath::SetRect(&rect, p_min, p_max);

	//rect.Width = p_max.x - p_min.x;
 //   rect.Height = p_max.y - p_min.y;
 //   rect.X = p_min.x + (rect.Width * 0.5f);
 //   rect.Y = p_min.y + (rect.Height * 0.5f);

}

void DebugRectLineComponent::SetRect(float p_x, float p_y, float p_width, float p_height) 
{ 
	rect.X = p_x;
    rect.Y = p_y;
    rect.Width = p_width;
    rect.Height = p_height;
}

void DebugRectLineComponent::Start() {

}

void DebugRectLineComponent::Update(float dt) 
{

}

void DebugRectLineComponent::Render(HDC hdc) 
{
    if (enabled == false) 
		return;

  //  if (!transform) 
		//return;

	if (active == false) 
		return;


    Gdiplus::Graphics graphics(hdc);

	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
	if ( m_ISTransformMatrix )
	{
        mainCamera->Begin_Update_GraphicsMatrix(graphics, transform, m_ISCameraMatrix);
	}
	else
	{
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.ResetTransform();
	}
    

    Gdiplus::Pen pen(color, thickness);
	graphics.DrawRectangle(&pen, rect);


	if (mainCamera)
	{
        mainCamera->End_Update_GraphicsMatrix(graphics, transform);
	}
	else
	{
        graphics.ResetTransform();
	}

}
