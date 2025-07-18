#include "Camera.h"
#include "../Core/GameObject.h"
#include "Transform.h"
#include "../Manager/CameraManager.h"



Gdiplus::Matrix& Camera::GetCameraWorldMatrix() 
{
    UpdateIfDirty();
    return cachedCameraWorldMatrix;
}

float Camera::GetWorldRotation() const
{
	return transform->GetWorldRotation();
}
Vec2& Camera::GetWorldPos( ) const
{
	return transform->GetWorldPosition();
}

void Camera::UpdateIfDirty() 
{ 
	if (!isDirty) return;


	// 위치와 회전까지는 부모 Transform에서 가져옴
	// 스케일은 카메라용 사용하기
	//m_TransformMatrix = &transform->GetWorldMatrix();


    Vec2& wpos = transform->GetWorldPosition();

	cachedCameraWorldMatrix.Reset();
    cachedCameraWorldMatrix.Scale(1.f / zoom, 1.f / zoom, Gdiplus::MatrixOrderAppend);
    cachedCameraWorldMatrix.Rotate(transform->GetWorldRotation(), Gdiplus::MatrixOrderAppend);
    cachedCameraWorldMatrix.Translate(-wpos.x, -wpos.y, Gdiplus::MatrixOrderAppend);

	isDirty = false;
}


void Camera::SetWorldPosition( Vec2& pos)
{ 
	if (!transform) return;

    MarkDirty();
    transform->SetWorldPosition(pos);
}
void Camera::SetWorldPosition(float p_x, float p_y) 
{
    if (!transform) return;

    MarkDirty();
    transform->SetWorldPosition(p_x, p_y);
}
void Camera::SetWorldRotation(float p_rot) {
    if (!transform) return;

    MarkDirty();
    transform->SetWorldRotation(p_rot);
}

void Camera::SetLocalPosition(const Vec2 &pos)
{
    if (!transform)
        return;

	MarkDirty();
    transform->setLocalPosition(pos);
}

void Camera::SetLocalPosition(float p_x, float p_y) 
{
    if (!transform) return;

	MarkDirty();
    transform->setLocalPosition(p_x, p_y);
}

Vec2 Camera::ApplyToPoint(const Vec2& worldPos) const
{
    Vec2 wpos = transform->GetWorldPosition();

    return { (worldPos.x - wpos.x) * zoom, (worldPos.y - wpos.y) * zoom };
}

void Camera::ApplyToMatrix(Gdiplus::Matrix& matrix) const 
{
    Vec2 wpos = transform->GetWorldPosition();
    matrix.Translate(-wpos.x, -wpos.y);
    matrix.Scale(zoom, zoom);
}

Gdiplus::Matrix& Camera::GetMultiplyMatrix( Gdiplus::Matrix& matrix ) 
{
 //   //UpdateIfDirty();

	//Vec2& wpos = transform->GetWorldPosition();
	//Vec2& viewportsize = CameraManager::GetI()->GetViewportCenter();



	////matrix.Translate(-wpos.x, -wpos.y, Gdiplus::MatrixOrderAppend);
	////matrix.Translate(-viewportsize.x, -viewportsize.y, Gdiplus::MatrixOrderAppend);
	////matrix.Scale(zoom, zoom, Gdiplus::MatrixOrderAppend);
 ////   matrix.Rotate(transform->GetWorldRotation(), Gdiplus::MatrixOrderAppend);
	////matrix.Translate(viewportsize.x, viewportsize.y, Gdiplus::MatrixOrderAppend);


 //   matrix.Scale(1.f / zoom, 1.f / zoom, Gdiplus::MatrixOrderAppend);
 //   matrix.Rotate(transform->GetWorldRotation(), Gdiplus::MatrixOrderAppend);
 //   matrix.Translate(-wpos.x, -wpos.y, Gdiplus::MatrixOrderAppend);


	Gdiplus::Matrix& cammatrix = GetCameraWorldMatrix();
	matrix.Multiply(&cammatrix, Gdiplus::MatrixOrderAppend);

	return matrix;
}

Camera::Camera() 
{ 
	//cachedWorldMatrix.SetElements(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	//cachedWorldMatrix.Reset();

 //   //GdiPlus::Status state = cachedWorldMatrix.GetLastStatus();


	//Gdiplus::Status status = cachedWorldMatrix.SetElements(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

	//Gdiplus::Matrix identityMatrix;
	//identityMatrix.Reset();
}

Camera::~Camera() {
    // transform = nullptr;
}

void Camera::Start()
{
}

void Camera::Update(float dt)
{
}

void Camera::Render(HDC hdc)
{
}

void Camera::Initialize_AddCompoment()
{
    __super::Initialize_AddCompoment();
}

void Camera::Initialize()
{
}

void Camera::Release()
{
}

void Camera::Destroy()
{ 

}


void Camera::Begin_Update_GraphicsMatrix(OUT Gdiplus::Graphics& graphics
	, Transform* p_transform
	, bool p_iscameramat
	, bool p_issmooth) 
{ 
	if (p_issmooth)
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	graphics.ResetTransform();
	

	Gdiplus::Matrix& transformmat = p_transform->GetWorldMatrix();
    graphics.MultiplyTransform(&transformmat, Gdiplus::MatrixOrderAppend);

	if (p_iscameramat)
	{
        Gdiplus::Matrix& campmat = GetCameraWorldMatrix();
        graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);
	}


}

void Camera::End_Update_GraphicsMatrix(OUT Gdiplus::Graphics& graphics, Transform* p_transform) 
{ 
	//Gdiplus::Matrix& campmat = GetCameraWorldMatrix();
    //Gdiplus::Matrix& transformmat = p_transform->GetWorldMatrix();
    //graphics.MultiplyTransform(&transformmat, Gdiplus::MatrixOrderAppend);
    //graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);

	graphics.ResetTransform();
}
