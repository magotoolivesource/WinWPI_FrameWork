#include "Plant_PeaShooter.h"
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
#include "InGameDefineDatas.h"

#include "Pea_Com.h"


using namespace std;

Plant_PeaShooter* Plant_PeaShooter::Create_PeaShooterObject( )
{
	GameObject* peashooterobj = InGame_PVSZ::AddGameObject("PeaShooter");
	peashooterobj->AddTag(TAG_PLANT);

	Plant_PeaShooter* peashooter_com = peashooterobj->AddComponent< Plant_PeaShooter>( );

	auto* imgcom = peashooterobj->AddComponent<ImageComponent>( );
	imgcom->ImageLoadImage(L"Assets/Images/Plants/Peashooter/Peashooter_1.png");
	imgcom->SetDrawRect(ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10, 0, 0, 73, 74);

	return peashooter_com;

}

Plant_PeaShooter::Plant_PeaShooter( )
{
}

Plant_PeaShooter::~Plant_PeaShooter( )
{

}

void Plant_PeaShooter::Start()
{
	__super::Start();

}

void Plant_PeaShooter::Update(float dt)
{
	__super::Update(dt);


}

void Plant_PeaShooter::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Plant_PeaShooter::Call_CreatePeaFN(UtilTimer* utiltimer, void* p_data)
{
	string msg = std::format("{} 탄발사 : "
		, this->owner->GetName( ));
	UtilLoger::Log(msg);

	Pea_Com* peacom = Pea_Com::Create_PeaComObject( );


	Vec2 offsetpos(50, 20);
	Vec2 temppos = this->transform->GetWorldPosition( ) + offsetpos;

	peacom->transform->SetWorldPosition(
		temppos
	);
	peacom->transform->SetDepth(( int ) E_ALLLayerType::Shooter);
}

void Plant_PeaShooter::InitSettings( )
{

	UtilTimerManager::GetI( )->AddTimer(4.f
		, nullptr //std::bind(&BaseGame::Test_AniCallBackFN, this, std::placeholders::_1)
		, std::bind(&Plant_PeaShooter::Call_CreatePeaFN, this, std::placeholders::_1, nullptr)
		, nullptr
		, -1
	);

}

void Plant_PeaShooter::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
