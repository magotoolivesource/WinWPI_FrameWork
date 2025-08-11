//#pragma once
#include "ImageComponent.h"
#include "../Core/GameObject.h"
#include "Transform.h"
#include "../Manager/ImageManager.h"
#include "../Manager/CameraManager.h"
#include "Camera.h"

#include "../Manager/DebugObjectManager.h"
#include "../Core/MyMath.h"
#include "../Core/DebugObject.h"
#include "../Core/DebugLineComponent.h"
#include "../Core/DebugRectLineComponent.h"



#pragma comment(lib, "gdiplus.lib")

ImageComponent::~ImageComponent()
{
    //GdiplusShutdown(gdiplusToken);
    // 에서 image들이 메모리 해제가됨
	if (m_Image) {
		//delete image;
		m_Image = nullptr;
	}
}

void ImageComponent::Initialize_AddCompoment()
{
    __super::Initialize_AddCompoment();

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


	m_Image = nullptr;
    m_Image = ImageManager::GetI()->Load(path);


    bool isimage = m_Image&& m_Image->GetLastStatus() == Gdiplus::Ok;

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

bool ImageComponent::SetImage(Gdiplus::Image* p_img)
{
	m_Image = p_img;
	return m_Image && m_Image->GetLastStatus( ) == Gdiplus::Ok;
}

Gdiplus::Image* ImageComponent::GetImage( )
{
	return m_Image;
}

//void ImageComponent::Render(HDC hdc)
//{
//    if ( !enabled ) return;
//
//
//	//Transform* temptransform = owner->GetComponent<Transform>();
//    //Transform* temptransform = owner->transform;
//    Transform* temptransform = transform;
//
//    if (!image || !temptransform) return;
//
//    Gdiplus::Graphics graphics(hdc);
//    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
//
//	//Vec2 temppos = temptransform->getLocalPosition();
// //   Gdiplus::PointF pos(temppos.x, temppos.y);
//
//
//	//float rot = temptransform->getRotation();
//
//	//graphics.TranslateTransform(pos.X, pos.Y);
//	//graphics.RotateTransform(temptransform->getRotation());
//    
//
//    //graphics.TranslateTransform(-pos.X, -pos.Y);
//
//	// 백업
//	//graphics.GetTransform(&m_TempTransMatrix);
//
//
//	temptransform->GetWorldMatrix().GetElements(matrixElements);
//    m_TempTransMatrix.SetElements(
//		matrixElements[0], matrixElements[1],
//		matrixElements[2], matrixElements[3],
//		matrixElements[4], matrixElements[5]);
//	//Gdiplus::Matrix tempmat(matrixElements);
//
//
//
//	graphics.ResetTransform();
//
//	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
//	if ( false )
//	{
//        mainCamera->GetMultiplyMatrix(m_TempTransMatrix);
//        graphics.SetTransform(&m_TempTransMatrix);
//	}
//	else
//	{
//        Gdiplus::Matrix& campmat = mainCamera->GetCameraWorldMatrix();
//
//        graphics.MultiplyTransform(&m_TempTransMatrix, Gdiplus::MatrixOrderAppend);
//        graphics.MultiplyTransform(&campmat, Gdiplus::MatrixOrderAppend);
//	}
//
//	
//
//
//    //int x = static_cast<int>(temppos.x);
//    //int y = static_cast<int>(temppos.y);
//    int w = drawWidth > 0 ? drawWidth : image->GetWidth();
//    int h = drawHeight > 0 ? drawHeight : image->GetHeight();
//
//    if (useDrawRect) {
//        Gdiplus::Rect destRect(0, 0, w, h);
//        Gdiplus::Rect srcRect(drawRect.left, drawRect.top,
//            drawRect.right - drawRect.left,
//            drawRect.bottom - drawRect.top);
//        graphics.DrawImage(image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
//    }
//    else {
//        graphics.DrawImage(image, 0, 0, w, h);
//    }
//
//
//    graphics.ResetTransform();
//
//	// 원복
//	//graphics.SetTransform( &m_TempTransMatrix );
//}


void ImageComponent::Update(float dt) 
{

	if ( m_ISDebugBoundBox )
	{
        Vec2& pos = transform->GetWorldPosition();
		//// uv 영역값
  //      float width = drawRect.right - drawRect.left;
  //      float height = drawRect.bottom - drawRect.top;

		float width = drawWidth > 0 ? drawWidth : m_Image->GetWidth();
        float height = drawHeight > 0 ? drawHeight : m_Image->GetHeight();

		Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
        //RectF boundbox;
        MyMath::GetBoundBox(&this->transform->GetWorldMatrix()
			, &mainCamera->GetCameraWorldMatrix()
			, width, height
			, &m_DebugBoundBox);
        DebugRectLineComponent* com = DebugObjectManager::Instance().DrawRectLine(
            m_DebugBoundBox.X, m_DebugBoundBox.Y, m_DebugBoundBox.Width, m_DebugBoundBox.Height
		);


	}

}

void ImageComponent::Render(HDC hdc) {
	if (!enabled) return;

    Transform* temptransform = transform;

    if (!m_Image || !temptransform) return;


    Gdiplus::Graphics graphics(hdc);
	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
    mainCamera->Begin_Update_GraphicsMatrix(graphics, temptransform);


    //int x = static_cast<int>(temppos.x);
    //int y = static_cast<int>(temppos.y);
    int w = drawWidth > 0 ? drawWidth : m_Image->GetWidth();
    int h = drawHeight > 0 ? drawHeight : m_Image->GetHeight();

    if (useDrawRect) {
        Gdiplus::Rect destRect(0, 0, w, h);
        Gdiplus::Rect srcRect(drawRect.left, drawRect.top, drawRect.right - drawRect.left, drawRect.bottom - drawRect.top);
        graphics.DrawImage(m_Image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
    } else {
        graphics.DrawImage(m_Image, 0, 0, w, h);
    }

	mainCamera->End_Update_GraphicsMatrix(graphics, temptransform);

	//Gdiplus::RectF* rectf;
}

void ImageComponent::GetDrawSRCNDESTRect(Gdiplus::Rect* p_src
	, Gdiplus::Rect* p_dest
	, bool* p_isdrawrect)
{
	*p_isdrawrect = useDrawRect;

	int w = drawWidth > 0 ? drawWidth : m_Image->GetWidth( );
	int h = drawHeight > 0 ? drawHeight : m_Image->GetHeight( );

	p_src->X = 0;
	p_src->Y = 0;
	p_src->Width = w;
	p_src->Height = h;

	if ( *p_isdrawrect )
	{
		p_dest->X = drawRect.left;
		p_dest->Y = drawRect.top;
		p_dest->Width = drawRect.right - drawRect.left;
		p_dest->Height = drawRect.bottom - drawRect.top;
	}
	else
	{
		p_dest->X = 0;
		p_dest->Y = 0;
		p_dest->Width = 0;
		p_dest->Height = 0;
	}
	

}

void ImageComponent::Draw(HDC hdc)
{
    
}
