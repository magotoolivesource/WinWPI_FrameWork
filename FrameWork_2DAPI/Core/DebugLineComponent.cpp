#include "DebugLineComponent.h"
#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"
#include "../Core/MyMath.h"


void DebugLineComponent::Start() {

}

void DebugLineComponent::Update(float dt) {

}

void DebugLineComponent::Render(HDC hdc) {
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
