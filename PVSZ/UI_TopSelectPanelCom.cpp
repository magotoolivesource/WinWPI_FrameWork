#include "UI_TopSelectPanelCom.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Compoment/ImageComponent.h>
#include <UICompoment/Button.h>
#include <Core/UtilLoger.h>

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"

using namespace std;

UI_TopSelectPanelCom::UI_TopSelectPanelCom( )
{
}

UI_TopSelectPanelCom::~UI_TopSelectPanelCom( )
{

}

void UI_TopSelectPanelCom::Start()
{
	__super::Start();

}

void UI_TopSelectPanelCom::Update(float dt)
{
	__super::Update(dt);


}

void UI_TopSelectPanelCom::Render(HDC hdc)
{
	__super::Render(hdc);


}

void UI_TopSelectPanelCom::Click_CallBackFN(Button* p_clickbtn, int p_selectid)
{
	UI_SelectBTNCom* btn = dynamic_cast< UI_SelectBTNCom* >( p_clickbtn );
	btn->GetCurrentPlantType( );

	//std::format("{}_버턴", i);
	wstring msg = format(L"{}, {} : 버턴 클릭 : "
		, (int)btn->GetCurrentPlantType( )
		, p_selectid );
	UtilLoger::Log(msg);

	m_BGIconCom->SetImage(btn->GetImage( ));
}

void UI_TopSelectPanelCom::InitSettings( )
{
	// Assets\Images\Cards\DayCards
	// Assets\Images\UI\sun_bg.png


	// bg 판넬
	auto panelbguiobj = InGame_PVSZ::AddGameObject("uibg");
	panelbguiobj->transform->SetParent(transform);
	panelbguiobj->transform->SetDepth(2);
	m_BGTopUIImage = panelbguiobj->AddComponent<ImageComponent>( );
	m_BGTopUIImage->ImageLoadImage( L"Assets/Images/UI/sun_bg.png" );
	m_BGTopUIImage->SetDrawRect(420, 82, 0, 0, 123, 34 );
	// 132 픽셀


	// 선택 판넬
	auto selectpanelbguiobj = InGame_PVSZ::AddGameObject("uibg_select");
	selectpanelbguiobj->transform->SetParent(transform);
	selectpanelbguiobj->transform->SetDepth(3);
	m_BGSelectCom = selectpanelbguiobj->AddComponent<ImageComponent>( );
	m_BGSelectCom->ImageLoadImage(L"Assets/Images/UI/sun_bg.png");
	m_BGSelectCom->SetDrawRect(90, 100, 35, 0, 117, 34);
	m_BGSelectCom->transform->setLocalPosition(400, 0);

	// 선택 아이콘
	auto selecticonobj = InGame_PVSZ::AddGameObject("ui_select_icon");
	selecticonobj->transform->SetParent(transform);
	selecticonobj->transform->SetDepth(4);
	m_BGIconCom = selecticonobj->AddComponent<ImageComponent>( );
	m_BGIconCom->SetImage(nullptr);
	m_BGIconCom->transform->setLocalPosition(400 + 10, 0 + 10);




	vector<wstring> cardimage = { L"Assets/Images/Cards/DayCards/SunFlower.png"
		, L"Assets/Images/Cards/DayCards/Peashooter.png"
		, L"Assets/Images/Cards/DayCards/SnowPea.png"
		, L"Assets/Images/Cards/DayCards/TallNut.png"};

	// 버턴들 
	Vec2 offsetpos = { 132 + 10, 10 };
	Vec2 cardsize = { 50, 60 };
	Vec2 offsetsolve = { 5, 0 };
	for ( size_t i = 0; i < 3; i++ )
	{
		std::string name = std::format("{}_버턴", i);
		GameObject* uiselectbtnobj = InGame_PVSZ::AddGameObject(name);
		UI_SelectBTNCom* selectbtncom = uiselectbtnobj->AddComponent<UI_SelectBTNCom>( );
		m_SelectBTNComVec.push_back(selectbtncom);

		selectbtncom->ImageLoadImage(cardimage[ i ]);
		selectbtncom->SetDrawRect(cardsize.x, cardsize.y, 0, 0, 100, 60);

		//selectbtncom->Button::transform->SetParent(this->transform);
		selectbtncom->Button::transform->SetParent(this->transform);
		selectbtncom->Button::transform->SetDepth(5 + i);

		float xx = offsetpos.x + ( i * cardsize.x ) + ( i * offsetsolve.x );
		selectbtncom->Button::transform->setLocalPosition(xx, offsetpos.y);
		selectbtncom->SetBTNSize(cardsize.x, cardsize.y);

		selectbtncom->SetCurrentPlantType(( E_PlantType ) i);

		//selectbtncom->OnClickCallBack(
		//	std::bind(UI_TopSelectPanelCom::Click_CallBackFN, this, std::placeholders::_1)
		//);
		selectbtncom->OnClickCallBack( [this, i]( Button* btn )
		{
			this->Click_CallBackFN(btn, i);
		}
		);

	}
	

}