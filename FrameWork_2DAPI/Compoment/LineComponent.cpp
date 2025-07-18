#include "LineComponent.h"

#include "../Core/Rect.h"
#include "Component.h"
#include "Transform.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include "../Core/Vector.h"

#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"


// https://gemini.google.com/app/92e317d273e6a9b5
// GDI+ 네임스페이스 사용
using namespace Gdiplus;


void LineComponent::Start() 
{

}

void LineComponent::Update(float dt) {

}

void LineComponent::Render(HDC hdc) { 

	if ( enabled == false ) return;

	if (!transform) return;

    Gdiplus::Graphics graphics(hdc);

	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
	if (m_ISTransformMatrix)
	{
        mainCamera->Begin_Update_GraphicsMatrix(graphics, transform, m_ISCameraMatrix);
	}
	else
	{
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.ResetTransform();
	}


	Gdiplus::Pen pen(color, thickness);
    graphics.DrawLine(&pen, start, end);


	if (m_ISTransformMatrix)
	{
        mainCamera->End_Update_GraphicsMatrix(graphics, transform);
	} 
	else
	{
		graphics.ResetTransform();
    }
		


}
