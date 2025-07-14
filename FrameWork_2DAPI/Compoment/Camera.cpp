#include "Camera.h"
#include "../Core/GameObject.h"
#include "Transform.h"




Gdiplus::Matrix& Camera::GetWorldMatrix() 
{
    UpdateIfDirty();
    return cachedWorldMatrix;
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

	cachedWorldMatrix.Reset();
    cachedWorldMatrix.Scale(zoom, zoom, Gdiplus::MatrixOrderAppend);
    cachedWorldMatrix.Rotate(transform->GetWorldRotation(), Gdiplus::MatrixOrderAppend);
    cachedWorldMatrix.Translate(wpos.x, wpos.y, Gdiplus::MatrixOrderAppend);

	isDirty = false;
}


void Camera::SetWorldPosition( Vec2& pos)
{ 
	if (!transform) return;

    MarkDirty();
    transform->SetWorldPosition(pos);
}
void Camera::SetWorldPosition(int p_x, int p_y) 
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

void Camera::SetLocalPosition(int p_x, int p_y) 
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
    UpdateIfDirty();

	Vec2 wpos = transform->GetWorldPosition();
	matrix.Scale(zoom, zoom, Gdiplus::MatrixOrderAppend);
    matrix.Rotate(transform->GetWorldRotation(), Gdiplus::MatrixOrderAppend);
    matrix.Translate(-wpos.x, -wpos.y, Gdiplus::MatrixOrderAppend);

	return matrix;
}

Camera::~Camera() 
{
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
