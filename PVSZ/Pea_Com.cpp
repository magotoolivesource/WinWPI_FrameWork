#include "Pea_Com.h"
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
#include <Compoment/Collider.h>
#include "Move_Com.h"

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"

#include "InGameDefineDatas.h"


using namespace std;

Pea_Com* Pea_Com::Create_PeaComObject( )
{
	GameObject* peaobj = InGame_PVSZ::AddGameObject("PeaCom");
	peaobj->AddTag(TAG_BULLET);


	Pea_Com* pea_com = peaobj->AddComponent< Pea_Com>( );

	float sizew = SHOOTERBLOCKSIZEW - 10;
	float sizeh = SHOOTERBLOCKSIZEH - 10;

	auto* imgcom = peaobj->AddComponent<ImageComponent>( );
	imgcom->ImageLoadImage(L"Assets/Images/Effect/PeaBullet.png");
	imgcom->SetDrawRect(sizew, sizeh
		, 0, 0, 56, 34
		, -( sizew ) * 0.5f
		, -( sizeh ) * 0.5f
	);


	Vec2 pos1(SHOOTERBLOCKSIZEW - 10, SHOOTERBLOCKSIZEH - 10);
	SizeF size1(SHOOTERBLOCKSIZEW - 10, SHOOTERBLOCKSIZEH - 10);
	BoxCollider* boxcol = peaobj->AddComponent<BoxCollider>(pos1, 0, size1, size1.Width, size1.Height);
	boxcol->SetCallFN( 
		std::bind(&Pea_Com::Collider_TriggerEnter, pea_com, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
		, nullptr
		, nullptr );


	Move_Com* movecom = peaobj->AddComponent<Move_Com>( );
	movecom->SetMoveSpeed(100, 0);


	return pea_com;

}

Pea_Com::Pea_Com( )
{
}

Pea_Com::~Pea_Com( )
{

}

void Pea_Com::Start()
{
	__super::Start();

}

void Pea_Com::Update(float dt)
{
	__super::Update(dt);


}

void Pea_Com::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Pea_Com::Collider_TriggerEnter(Collider* src_other
	, Collider* dest_other
	, void* p_data)
{
	string wlog = std::format( "pea 충돌 : src_other : {}, {}, dest_other : {}, {}", 
		src_other->owner->GetName( ), src_other->owner->GetTag()
		, dest_other->owner->GetName( ), dest_other->owner->GetTag() );
	UtilLoger::Log( wlog );

}

void Pea_Com::InitSettings( )
{
	
}

void Pea_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
