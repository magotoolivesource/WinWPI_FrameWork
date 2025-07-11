//#pragma once
#include "ImageComponent.h"
#include "../Core/GameObject.h"
#include "Transform.h"
#include "../Manager/ImageManager.h"

#pragma comment(lib, "gdiplus.lib")

ImageComponent::~ImageComponent()
{
    //GdiplusShutdown(gdiplusToken);
    // 에서 image들이 메모리 해제가됨
	if (image) {
		//delete image;
		image = nullptr;
	}
}

void ImageComponent::Initialize_AddCompoment()
{
    transform = owner->GetComponent<Transform>();
    //drawRect = { 0, 0, 32, 32 };
}
void ImageComponent::Initialize()
{
    
}

bool ImageComponent::ImageLoadImage(const std::wstring& path)
{
    //if (image) {
    //    delete image;
    //    image = nullptr;
    //}
    //image = new Gdiplus::Image(path.c_str());


	image = nullptr;
    image = ImageManager::GetI()->Load(path);


    bool isimage = image&& image->GetLastStatus() == Gdiplus::Ok;

 //   if (isimage) {
 //       drawWidth = image->GetWidth();
 //       drawHeight = image->GetHeight();
 //   }
 //   else {
 //       drawWidth = 0;
 //       drawHeight = 0;
	//}

    return isimage;
}

void ImageComponent::Render(HDC hdc)
{
    if ( !enabled ) return;


	//Transform* temptransform = owner->GetComponent<Transform>();
    //Transform* temptransform = owner->transform;
    Transform* temptransform = transform;

    if (!image || !temptransform) return;

    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	Vec2 temppos = temptransform->getLocalPosition();
    Gdiplus::PointF pos(temppos.x, temppos.y);


	float rot = temptransform->getRotation();

	//graphics.TranslateTransform(pos.X, pos.Y);
	//graphics.RotateTransform(temptransform->getRotation());
    

    //graphics.TranslateTransform(-pos.X, -pos.Y);

	// 백업
	//graphics.GetTransform(&m_TempTransMatrix);


	graphics.ResetTransform();
	graphics.SetTransform(&temptransform->GetWorldMatrix());


    int x = static_cast<int>(temppos.x);
    int y = static_cast<int>(temppos.y);
    int w = drawWidth > 0 ? drawWidth : image->GetWidth();
    int h = drawHeight > 0 ? drawHeight : image->GetHeight();

    if (useDrawRect) {
        Gdiplus::Rect destRect(0, 0, w, h);
        Gdiplus::Rect srcRect(drawRect.left, drawRect.top,
            drawRect.right - drawRect.left,
            drawRect.bottom - drawRect.top);
        graphics.DrawImage(image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
    }
    else {
        graphics.DrawImage(image, 0, 0, w, h);
    }


    graphics.ResetTransform();

	// 원복
	//graphics.SetTransform( &m_TempTransMatrix );
}

void ImageComponent::Draw(HDC hdc)
{
    
}
