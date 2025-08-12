#include "Stage01.h"
#include "Core/GameObject.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/Transform.h"
#include "Compoment/RectLineComponent.h"
#include "PlantSelectCom.h"
#include "UI_TopSelectPanelCom.h"
#include "PlantSelectCom.h"


Stage01::~Stage01( )
{
}

void Stage01::PrevInitSettings()
{
	__super::PrevInitSettings();
}

void Stage01::InitSettings()
{
	__super::InitSettings();

	InitBGCom( );
	InitSelectCom();
	InitHeaderCard();
}

void Stage01::InitSelectCom()
{
	Gdiplus::RectF rectF(100, 100, 200, 100);
	GameObject* linerect = this->CreateObject("linerect");
	PlantSelectCom* rectlinecom = linerect->AddComponent<PlantSelectCom>();
	rectlinecom->SetIsTransformMatrix(true);
	

	m_PlantSelectCom = rectlinecom;

}

void Stage01::InitBGCom()
{
	// 배경 그리기
	auto gbobj = this->CreateObject("BG");
	auto bgcom = gbobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	bgcom->ImageLoadImage(L"Assets/Images/Background/background1.jpg");
	bgcom->ResetUseDrawRect( );
	//bgcom->SetDrawRect()
	bgcom->transform->SetWorldPosition(0, 0);
	bgcom->transform->SetDepth(-100);
}

void Stage01::InitHeaderCard()
{
	auto uipanel = this->CreateObject("판넬");

	UI_TopSelectPanelCom* topuipanelcom = uipanel->AddComponent<UI_TopSelectPanelCom>( );
	topuipanelcom->transform->SetDepth(10);
	topuipanelcom->InitSettings( );
	

	// 기본 등록 하기용
	m_PlantSelectCom->m_TopSelectPanelCom = topuipanelcom;
}
