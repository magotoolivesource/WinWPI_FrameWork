#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "Component.h"


class Transform;

class ImageComponent : public Component 
{
protected:
    Gdiplus::Image* m_Image;
    //Transform* transform;
    int drawWidth, drawHeight;
    RECT drawRect = { 0, 0, 128, 128 };
    bool useDrawRect = false;


	Gdiplus::Matrix m_TempTransMatrix;
	Gdiplus::REAL matrixElements[6];


	bool m_ISDebugBoundBox = false;
    Gdiplus::RectF m_DebugBoundBox;


public:
    bool GetISDebugBoundBox() const { return m_ISDebugBoundBox; }
    void SetISDebugBoundBox(bool value) { m_ISDebugBoundBox = value; }


public:
    ImageComponent()
        : m_Image(nullptr), drawWidth(0), drawHeight(0), useDrawRect(false)
    {
        drawRect = {0, 0, 128, 128};
        //Initialize();
    }

    ImageComponent(Gdiplus::Image* p_img,
        int p_width, int p_height
        , bool p_useDrawRect
    ) : drawWidth(p_width), drawHeight(p_height), useDrawRect(p_useDrawRect)
    {
        m_Image = p_img;;
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
	bool SetImage(Gdiplus::Image* p_img);
	Gdiplus::Image* GetImage( );

	void SetDrawRect(int p_drawwidth, int p_drawheight
		, int p_imgleft, int p_imgtop, int p_imgright, int p_imgbottom)
	{
		drawWidth = p_drawwidth;
		drawHeight = p_drawheight;

		drawRect = { p_imgleft, p_imgtop, p_imgright, p_imgbottom };
		useDrawRect = true;
	}
	void SetDrawRect(int left, int top, int right, int bottom) {
		drawRect = { left, top, right, bottom };
		useDrawRect = true;
	}
    void SetSize(int width, int height) {
        drawWidth = width;
        drawHeight = height;
    }

	void ResetUseDrawRect( )
	{
		useDrawRect = false;
	}

	virtual void Update(float dt) override;
    virtual void Render(HDC hdc)  override;

	virtual void GetDrawSRCNDESTRect(Gdiplus::Rect* p_src
		, Gdiplus::Rect* p_dest
		, bool* p_isdrawrect);

    void Draw(HDC hdc);
};