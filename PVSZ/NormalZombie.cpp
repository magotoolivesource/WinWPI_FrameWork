#include "NormalZombie.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>
#include <string>
#include <format>


#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/ImageAni_Component.h>
#include <Compoment/Collider.h>
#include <UICompoment/Button.h>
#include <Core/UtilLoger.h>

#include <Manager/DebugObjectManager.h>
#include <Core/UtilTimerManager.h>
//#include <Core/TimerManager.h>


#include "InGameDefineDatas.h"
#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"
#include "Move_Com.h"

using namespace std;

NormalZombie::NormalZombie( )
{
}

NormalZombie::~NormalZombie( )
{

}

void NormalZombie::Start()
{
	__super::Start();


	// 이쪽에서 새롭개 생성하면 start에서 호출 되면서 pendingObjects에 실시간 추가되면서 문제가 됨
	//InitZombiSetting( );
}

void NormalZombie::Update(float dt)
{
	__super::Update(dt);


	UpdateUI( );


	// 화면 밖으로 나가면 삭제 적용
	Vec2 temppos = transform->GetWorldPosition( );

	if ( temppos.x < 300 || temppos.x > SCREEN_W - 100 
		)
	{
		InGame_PVSZ::DestroyGameObject(this->owner);
		return;
	}
}

void NormalZombie::Render(HDC hdc)
{
	__super::Render(hdc);

}

void NormalZombie::InitSettings( )
{
	__super::InitSettings( );



}

void NormalZombie::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	//InitSettings( );

	InitZombiSetting( );
	
}

void NormalZombie::InitZombiSetting( )
{
	ImageComponent* imgcom = this->owner->AddComponent<ImageComponent>( );// ->ImageLoadImage(L"Assets/Images/Zombies/Zombie/Zombie_1.png");
	imgcom->ImageLoadImage(L"Assets/Images/Zombies/Zombie/Zombie_1.png");
	imgcom->SetDrawRect(
		ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10,
		0, 0, 166, 144,
		-( ONEBLOCKSIZEW - 10 ) * 0.5f,
		-( ONEBLOCKSIZEH - 10 ) * 0.5f
	);
	m_ImageCom = imgcom;


	InitSettingBaseAnimations( );


	m_MoveSpeed = -30.f;
	Move_Com* movecom = this->owner->AddComponent<Move_Com>( );
	movecom->SetMoveSpeed(m_MoveSpeed, 0); // 좀비 이동 속도 설정
	m_MoveCom = movecom;


	this->owner->AddTag(TAG_ZOMBIE); // 좀비 태그 추가

	BoxCollider* boxcol = this->owner->AddComponent<BoxCollider>(
		Vec2(ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10), 0,
		SizeF(ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10),
		ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10
	);

	boxcol->SetCallFN(
		std::bind(&NormalZombie::Collider_TriggerEnter, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
		, nullptr
		, std::bind(&NormalZombie::Collider_TriggerExit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	);

}

void NormalZombie::Collider_TriggerEnter(Collider* src_other
	, Collider* dest_other
	, void* p_data)
{
	string tag = dest_other->owner->GetTag( );

	if ( tag != TAG_PLANT )
		return;


	m_TargetPlant = dest_other->owner;
}

void NormalZombie::Collider_TriggerStay(Collider* src_other, Collider* dest_other, void* p_data)
{

}

void NormalZombie::Collider_TriggerExit(Collider* src_other
	, Collider* dest_other
	, void* p_data)
{
	string tag = dest_other->owner->GetTag( );
	if ( tag != TAG_PLANT )
		return;

	// 해지하기
	if ( dest_other->owner == m_TargetPlant )
	{
		m_TargetPlant = nullptr;
	}
	

	// 범위안에 다른 식물 있는지 파악 하기용
}

void NormalZombie::SetDieEvent( )
{
	m_ISDie = true;

	if ( m_ImageCom )
	{
		m_ImageCom->ImageLoadImage(L"Assets/Images/Zombies/Zombie/BoomDie_1.png");
	}

	//m_MoveCom->SetMoveSpeed(0, 0);
	SetMove(false);
	Set_DieAnimations( );

	UtilTimerManager::GetI( )->AddTimer(
		7.f
		, nullptr
		, std::bind(&NormalZombie::DestroyEvent_CallFN, this, std::placeholders::_1, nullptr)
		, nullptr
		, 0
	);
}

void NormalZombie::DestroyEvent_CallFN(UtilTimer* p_timer, void* p_data)
{
	string wlog = std::format("좀비삭제 : {}", this->owner->GetName( ) );
	UtilLoger::Log(wlog);

	InGame_PVSZ::DestroyGameObject(this->owner);

}

void NormalZombie::InitSettingBaseAnimations( )
{
	ImageAni_Component* aniimgcom = this->owner->AddComponent<ImageAni_Component>( );
	m_LinkImaeAniCom = aniimgcom;

	// 22개
	wstring path = L"";
	for ( size_t i = 0; i < 22; i++ )
	{
		path = format(L"Assets/Images/Zombies/Zombie/Zombie_{}.png", i + 1);
		aniimgcom->AddDrawImageInfo(0.2f, path
			, ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10,
			0, 0, 166, 144,
			-( ONEBLOCKSIZEW - 10 ) * 0.5f,
			-( ONEBLOCKSIZEH - 10 ) * 0.5f);
	}

	aniimgcom->SetNAdjustImageInfoTotalFrame(3.f);
	aniimgcom->SetLoopTime(-1); // 무한 루프
	aniimgcom->SetISPlayAni(true);
	aniimgcom->SetCurrentIndex(0);
	aniimgcom->SetWeight_AnimationVal(1.5f);
}

void NormalZombie::Set_DieAnimations( )
{
	//ImageAni_Component* aniimgcom = this->owner->GetComponent<ImageAni_Component>( );
	ImageAni_Component* aniimgcom = m_LinkImaeAniCom;

	aniimgcom->ResetAllDatasNClear( );

	// 22개
	wstring path = L"";
	for ( size_t i = 0; i < 20; i++ )
	{
		path = format(L"Assets/Images/Zombies/Zombie/BoomDie_{}.png", i + 1);
		aniimgcom->AddDrawImageInfo(0.2f, path
			, ONEBLOCKSIZEW - 10, ONEBLOCKSIZEH - 10,
			0, 0, 166, 144,
			-( ONEBLOCKSIZEW - 10 ) * 0.5f,
			-( ONEBLOCKSIZEH - 10 ) * 0.5f);
	}

	aniimgcom->SetNAdjustImageInfoTotalFrame(2.f);
	aniimgcom->SetLoopTime(1); // 무한 루프
	aniimgcom->SetISPlayAni(true);
	aniimgcom->SetCurrentIndex(0);
	aniimgcom->SetWeight_AnimationVal(1.5f);
}

void NormalZombie::SetDamage(float p_atk)
{
	if ( m_ISDie )
		return;


	__super::SetDamage(p_atk);

	m_ZombiStateData.HP -= (int)p_atk;
	if ( m_ZombiStateData.HP <= 0 )
	{
		m_ISDie = true;
		SetDieEvent( );
	}

	UpdateUI( );
}

void NormalZombie::UpdateUI( )
{
	if ( ISDEBUG )
	{
		std::string msg = std::format("H:{}", m_ZombiStateData.HP);
		Vec2 pos = this->transform->GetWorldPosition( );
		pos.x += 10;
		pos.y += 20;
		DebugObjectManager::Instance( ).DrawDebugText(msg, pos, Gdiplus::Color::Chartreuse);
	}

}

void NormalZombie::SetMove(bool p_ismove)
{
	__super::SetMove(p_ismove);

	if ( p_ismove )
	{
		m_MoveCom->SetMoveSpeed(m_MoveSpeed, 0);
	}
	else
	{
		m_MoveCom->SetMoveSpeed(0, 0);
	}
}

void NormalZombie::DestroyEvent( )
{
}
