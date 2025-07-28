#include "RigidbodyComponent.h"
#include "../Compoment/Camera.h"
#include "../Manager/CameraManager.h"

void RigidbodyComponent::Start()
{

}

void RigidbodyComponent::Update(float dt)
{

}

void RigidbodyComponent::Render(HDC hdc)
{
    if (enabled == false) return;

    if (!transform) return;

    Gdiplus::Graphics graphics(hdc);



}
