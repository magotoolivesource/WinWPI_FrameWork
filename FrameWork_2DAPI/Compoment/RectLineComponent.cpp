#include "RectLineComponent.h"
#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"

void RectLineComponent::Start() 
{

}

void RectLineComponent::Update(float dt) 
{

}

void RectLineComponent::Render(HDC hdc) 
{
    if (enabled == false) return;

    if (!transform) return;

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
