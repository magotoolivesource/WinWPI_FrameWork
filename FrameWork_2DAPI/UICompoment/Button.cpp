//#pragma once
#include "Button.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Compoment/Camera.h"
#include "../Core/InputManager.h"
#include "../Manager/CameraManager.h"

#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

bool Button::ISClick() 
{ 
	if ( false )
	{
        Transform* t = this->transform; // owner->GetComponent<Transform>();
        if (!t) 
			return false;
        Vec2 wpos = t->GetWorldPosition();

        POINT mouse = InputManager::mousePosition;
        bool inside = mouse.x >= wpos.x && mouse.x <= wpos.x + t->width && mouse.y >= wpos.y && mouse.y <= wpos.y + t->height;

        return inside;
	}
	

	Transform* t = this->transform; // owner->GetComponent<Transform>();
	if (!t) return false;
	Vec2& wpos = t->GetWorldPosition();
    Vec2& wscale = t->GetWorldScale();
    float wrot = t->GetWorldRotation();
	POINT mouse = InputManager::mousePosition;

	Gdiplus::REAL centerX = wpos.x + t->width * 0.5f;
    Gdiplus::REAL centerY = wpos.x + t->width * 0.5f;


	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
    Gdiplus::Matrix& campmat = mainCamera->GetCameraWorldMatrix();

	Gdiplus::REAL matrixElements[6];
    Gdiplus::Matrix wmat;
    t->GetWorldMatrix().GetElements(matrixElements);
    wmat.SetElements(
        matrixElements[0], matrixElements[1]
		, matrixElements[2], matrixElements[3]
		, matrixElements[4], matrixElements[5]);

	wmat.Multiply(&campmat, Gdiplus::MatrixOrderAppend);

	//Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
 //   Gdiplus::Matrix& campmat = mainCamera->GetCameraWorldMatrix();
 //   graphics.MultiplyTransform(&m_TempTransMatrix, Gdiplus::MatrixOrderAppend);
 //   graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);


	Status status2 = wmat.Invert();
    if (status2 != Ok) {
        // 역행렬 계산 실패 (예: 행렬이 특이 행렬인 경우)
        return false;
    }

	PointF ptMouseF2((REAL)mouse.x, (REAL)mouse.y);
    // 4. 마우스 포인터에 역변환 행렬을 적용하여 로컬 좌표를 얻습니다.
    wmat.TransformPoints(&ptMouseF2, 1);

	return (ptMouseF2.X >= 0
		&& ptMouseF2.X <= (0 + t->width) 
		&& ptMouseF2.Y >= 0 
		&& ptMouseF2.Y <= (0 + t->height));





	// 1. 객체의 변환 행렬을 생성합니다. (그리기 시 사용했던 것과 동일)
    Gdiplus::Matrix transformMatrix;

	//// 스케일 적용 (중심 스케일을 위한 이동 포함)
	//transformMatrix.Translate(centerX, centerY);
 //   transformMatrix.Scale(wscale.x, wscale.y);
 //   transformMatrix.Translate(-centerX, -centerY);
	
	// 회전 적용(중심 회전을 위한 이동 포함) 
	transformMatrix.Translate(centerX, centerY);
    transformMatrix.Rotate(wrot);
    transformMatrix.Translate(-centerX, -centerY);



	// 명확성을 위해 순서를 다시 정렬: 스케일 -> 회전 -> 이동
    Matrix finalMatrix;
    finalMatrix.Scale(wscale.x, wscale.y); // 스케일 적용


	// GDI+ Matrix를 이용한 변환은 누적되므로 순서가 중요합니다.
    // 여기서는 최종적으로 하나의 Matrix를 만들고 Invert()를 사용합니다.
    Matrix fullTransform;
    //fullTransform.Translate(wpos.x, wpos.y); // 사각형의 좌상단으로 이동 (기준점)

	// 스케일은 기준점 (x,y)에서부터 적용됩니다.
    fullTransform.Scale(wscale.x, wscale.y);


	// 회전은 스케일된 사각형의 중심을 기준으로 해야 합니다.
    // 회전 중심: 스케일된 사각형의 원래 중심점으로부터의 상대적인 위치
    //// (width * scaleX / 2), (height * scaleY / 2)
    //REAL scaledCenterX = wpos.x + (t->width * wscale.x) / 2.0f;
    //REAL scaledCenterY = wpos.y + (t->height * wscale.y) / 2.0f;

	REAL scaledCenterX = wpos.x + (t->width * wscale.x);
    REAL scaledCenterY = wpos.y + (t->height * wscale.y);

    // 중심점으로 이동, 회전, 다시 되돌리기
    fullTransform.Translate(scaledCenterX, scaledCenterY);
    //fullTransform.Scale(wscale.x, wscale.y);
    fullTransform.Rotate(wrot);
    fullTransform.Translate(-scaledCenterX, -scaledCenterY);

	// 2. 변환 행렬의 역행렬을 계산합니다.
    // 마우스 좌표 (화면 좌표)를 객체의 로컬 좌표로 변환하기 위함입니다.
    Status status = fullTransform.Invert();
    if (status != Ok) {
        // 역행렬 계산 실패 (예: 행렬이 특이 행렬인 경우)
        return false;
    }

	// 3. 마우스 포인터 좌표를 PointF로 변환합니다.
    PointF ptMouseF((REAL)mouse.x, (REAL)mouse.y);

    // 4. 마우스 포인터에 역변환 행렬을 적용하여 로컬 좌표를 얻습니다.
    fullTransform.TransformPoints(&ptMouseF, 1);

	// 5. 역변환된 마우스 좌표가 원래 사각형 범위 내에 있는지 확인합니다.
    // 사각형은 (x,y)에서 시작하여 width, height를 가집니다.
    // 주의: 위의 `DrawTransformedRectangleGDIPlus`에서 `graphics->FillRectangle(&solidBrush, x, y, width, height);`를 사용했으므로,
    // 이 사각형의 논리적 위치는 (x,y)입니다.
    // 따라서 역변환된 마우스 좌표가 이 (x,y)를 기준으로 한 사각형 내에 있는지 확인합니다.
    return (ptMouseF.X >= wpos.x 
		&& ptMouseF.X <= (wpos.x + t->width) 
		&& ptMouseF.Y >= wpos.y 
		&& ptMouseF.Y <= (wpos.y + t->height));
}

void Button::Update(float dt) {
    Transform* t = this->transform; // owner->GetComponent<Transform>();
    if (!t) return;

	Vec2 wpos = t->GetWorldPosition( );

    POINT mouse = InputManager::mousePosition;
    bool inside = mouse.x >= wpos.x && mouse.x <= wpos.x + t->width &&
        mouse.y >= wpos.y && mouse.y <= wpos.y + t->height;

    if ( ISClick() ) {
        isHovered = true;
        if (InputManager::mouseLeftDown && !isPressed) {
            isPressed = true;
        }
        else if (!InputManager::mouseLeftDown && isPressed) {
            isPressed = false;
            //if (onClick) onClick(this);
        }

		//if ( InputManager::ISMouseUp( E_MOUSEETYPE::LEFT ) )
		//{
  //          if (onClick) onClick(this);
		//}

		if ( InputManager::ISMouseDown(E_MOUSEETYPE::LEFT) ) {
            if (onClick) onClick(this);
        }
    }
    else {
        isHovered = false;
        isPressed = false;
    }
}

void Button::Render(HDC hdc)
{
	
    Transform* t = this->transform; // owner->transform; // owner->GetComponent<Transform>();
    if (!t) return;


	if ( false )
	{
        Vec2 wpos = t->GetWorldPosition();

        HBRUSH brush;
        if (isPressed)
            brush = CreateSolidBrush(RGB(200, 200, 200));
        else if (isHovered)
            brush = CreateSolidBrush(RGB(220, 220, 220));
        else
            brush = CreateSolidBrush(RGB(240, 240, 240));

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        Rectangle(hdc, (int)wpos.x, (int)wpos.y, (int)(wpos.x + t->width), (int)(wpos.y + t->height));
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);

		return;
	}
	


	Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);


	//static Gdiplus::Color normalColor = Color(255, 240, 240, 240); // ARGB
    Gdiplus::SolidBrush gdibrush(m_DefaultColor);
    if (isPressed)
        gdibrush.SetColor(m_PressColor);
	else if (isHovered)
        gdibrush.SetColor(m_HoverColor);


	t->GetWorldMatrix().GetElements(matrixElements);
    m_TempTransMatrix.SetElements(
        matrixElements[0], matrixElements[1]
		, matrixElements[2], matrixElements[3]
		, matrixElements[4], matrixElements[5]);

	graphics.ResetTransform();

	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
	Gdiplus::Matrix& campmat = mainCamera->GetCameraWorldMatrix();

    graphics.MultiplyTransform(&m_TempTransMatrix, Gdiplus::MatrixOrderAppend);
    graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);
	
	Vec2 wpos = t->GetWorldPosition();
	graphics.FillRectangle(&gdibrush, 
		(float)0, (float)0, (float)t->width, (float)t->height
	);


	graphics.ResetTransform();
}