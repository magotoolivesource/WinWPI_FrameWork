#include "Camera.h"
#include "../Core/GameObject.h"
#include "Transform.h"

void Camera::SetLocalPosition(const Vec2 &pos)
{
    if (!transform)
        return;

    transform->setLocalPosition(pos);
}

void Camera::SetLocalPosition(int p_x, int p_y) 
{
    if (!transform) return;

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
