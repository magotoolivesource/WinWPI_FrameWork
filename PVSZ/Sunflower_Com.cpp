#include "Sunflower_Com.h"
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
//#include <Core/TimerManager.h>
#include <Core/UtilTimerManager.h>
#include <Core/MyUtil.h>

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"
#include "Sun_Com.h"
#include "InGameDefineDatas.h"


using namespace std;

Sunflower_Com* Sunflower_Com::CreateSunflower_Com( )
{
	GameObject* sunflowerobj = InGame_PVSZ::AddGameObject("SunFlower1");
	sunflowerobj->AddTag(TAG_PLANT);


	Sunflower_Com* sunflower_com = sunflowerobj->AddComponent< Sunflower_Com>( );

	auto* imgcom = sunflowerobj->AddComponent<ImageComponent>( );
	imgcom->ImageLoadImage(L"Assets/Images/Plants/SunFlower/SunFlower_1.png");
	//IMAGEPATH("Effect/Sun/Sun_1.png");
	imgcom->SetDrawRect(ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10, 0, 0, 73, 74);

	return sunflower_com;

}

Sunflower_Com::Sunflower_Com( )
{
}

Sunflower_Com::~Sunflower_Com( )
{

}

void Sunflower_Com::Start()
{
	__super::Start();

}

void Sunflower_Com::Update(float dt)
{
	__super::Update(dt);


}

void Sunflower_Com::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Sunflower_Com::Call_CreateSunFN(UtilTimer* utiltimer, void* p_data)
{
	wstring msg = std::format(L"{} 해바라기 시간마다 호출됨 : "
		, this->owner->GetNameW( ) );
	UtilLoger::Log(msg);


	// 추가 적용하기
	//GameObject* sunobj = InGame_PVSZ::AddGameObject("Sun");
	//Sun_Com* suncom = sunobj->AddComponent< Sun_Com>( );

	Sun_Com* suncom = Sun_Com::Create_SunObject( );


	float randx = MyUtil::GetRandFloat(-40.f, 40.f);
	float randy = MyUtil::GetRandFloat(-40.f, 40.f);

	Vec2 randpos(randx, randy);
	randpos += this->transform->GetWorldPosition( );

	suncom->transform->SetWorldPosition(
		randpos
	);
	suncom->transform->SetDepth( (int)E_ALLLayerType::SUN );

}

void Sunflower_Com::InitSettings( )
{
	UtilTimerManager::GetI( )->AddTimer(3.f
		, nullptr //std::bind(&BaseGame::Test_AniCallBackFN, this, std::placeholders::_1)
		, std::bind(&Sunflower_Com::Call_CreateSunFN, this, std::placeholders::_1, nullptr)
		, nullptr
		, -1
	);


}

void Sunflower_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );
	InitSettings( );

}
