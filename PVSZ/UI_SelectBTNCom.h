#pragma once
#include <string>
#include <Compoment/Component.h>
#include <UICompoment/Button.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <Core/DefineHeader.h>


#include "PVSZ_AllContentDatas.h"


class UI_SelectBTNCom : public Button//, public ImageComponent
{
public:
	UI_SelectBTNCom() = default;
	virtual ~UI_SelectBTNCom( ) override;

protected:
	ImageComponent* m_IconImg;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;

protected:
	// https://chatgpt.com/c/689982c0-33dc-832d-95d2-0863e3956080
	// 특정 색상 적용 방식
	void ButtonRender(HDC hdc);

protected:
	GETSETER(E_PlantType, CurrentPlantType, E_PlantType::MAX);

	GETSETER(bool, ISSelect, false);

public:
	virtual void Initialize( ) override;
	virtual void Initialize_AddCompoment( ) override;

	Gdiplus::Image* GetImage( );

	void ImageLoadImage(std::wstring p_loadimg);
	void SetDrawRect(int p_drawwidth, int p_drawheight
		, int p_imgleft, int p_imgtop, int p_imgright, int p_imgbottom);


};

