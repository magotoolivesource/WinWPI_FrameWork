#include "Sun_Com.h"
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
#include <Core/UtilTimerManager.h>
#include <Core/TimerManager.h>
#include <Compoment/Collider.h>
#include <Manager/CollisionManager.h>

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"
#include "InGameDefineDatas.h"

using namespace std;

Sun_Com* Sun_Com::Create_SunObject( )
{
	GameObject* sunobj = InGame_PVSZ::AddGameObject("Sun");
	Sun_Com* suncom = sunobj->AddComponent< Sun_Com>( );

	auto* imgcom = sunobj->AddComponent<ImageComponent>( );
	imgcom->ImageLoadImage( L"Assets/Images/Effect/Sun/Sun_1.png" );
	//IMAGEPATH("Effect/Sun/Sun_1.png");
	imgcom->SetDrawRect(50, 50, 0, 0, 78, 78);


	Vec2 pos1(50, 50);
	SizeF size1(50, 50);
	auto* boxcol = sunobj->AddComponent<BoxCollider>( );
	boxcol->SetBoxCollider(pos1, 0, size1, size1.Width, size1.Height);

	suncom->m_LinkBoxCollider = boxcol;

	return suncom;
}

Sun_Com::Sun_Com( )
{
}

Sun_Com::~Sun_Com( )
{

}

void Sun_Com::Start()
{
	__super::Start();

}

void Sun_Com::Update(float dt)
{
	__super::Update(dt);


	
	if ( InputManager::ISMouseDown(E_MOUSEETYPE::LEFT) )
	{
		if ( this->m_LinkBoxCollider )
		{
			if ( CollisionManager::ISBoxCollider(this->m_LinkBoxCollider, InputManager::GetMousePosition( )) )
			{
				wstring msg = std::format(L"({}) 햇빛 클릭 : "
					, this->owner->GetNameW( ));
				UtilLoger::Log(msg);

				InGame_PVSZ::DestroyGameObject(this->owner);
			}

		}
	}
	

}

void Sun_Com::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Sun_Com::InitSettings( )
{
	// 컴포넌트들 추가


	// 타이머 추가 적용


}

void Sun_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
