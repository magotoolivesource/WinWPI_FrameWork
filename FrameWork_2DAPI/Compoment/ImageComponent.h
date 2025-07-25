﻿#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "Component.h"


class Transform;

class ImageComponent : public Component 
{
private:
    Gdiplus::Image* image;
    Transform* transform;
    int drawWidth, drawHeight;
    RECT drawRect = { 0, 0, 128, 128 };
    bool useDrawRect;


	Gdiplus::Matrix m_TempTransMatrix;
	Gdiplus::REAL matrixElements[6];


	bool m_ISDebugBoundBox = false;
    Gdiplus::RectF m_DebugBoundBox;


public:
    bool GetISDebugBoundBox() const { return m_ISDebugBoundBox; }
    void SetISDebugBoundBox(bool value) { m_ISDebugBoundBox = value; }


public:
    ImageComponent()
        : image(nullptr), transform(nullptr), drawWidth(0), drawHeight(0), useDrawRect(false)
    {
        drawRect = {0, 0, 128, 128};
        //Initialize();
    }

    ImageComponent(Gdiplus::Image* p_img,
        int p_width, int p_height
        , bool p_useDrawRect
    ) : transform(nullptr), drawWidth(p_width), drawHeight(p_height), useDrawRect(p_useDrawRect)
    {
        image = p_img;;
        //Initialize();
    }

    virtual ~ImageComponent() override;

  //  void SetTransform(Transform* t) 
  //  {
		////this->owner
  //      transform = t;
  //  }

    virtual void Initialize_AddCompoment() override;
    virtual void Initialize() override;

    bool ImageLoadImage(const std::wstring& path);

    void SetSize(int width, int height) {
        drawWidth = width;
        drawHeight = height;
    }

    void SetDrawRect(int left, int top, int right, int bottom) {
        drawRect = { left, top, right, bottom };
        useDrawRect = true;
    }

	virtual void Update(float dt) override;
    virtual void Render(HDC hdc)  override;

    void Draw(HDC hdc);
};