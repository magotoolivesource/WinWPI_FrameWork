#include "Actor_State_Com.h"
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

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"

using namespace std;

Actor_State_Com::Actor_State_Com( )
{
}

Actor_State_Com::~Actor_State_Com( )
{

}

void Actor_State_Com::Start()
{
	__super::Start();

}

void Actor_State_Com::Update(float dt)
{
	__super::Update(dt);


}

void Actor_State_Com::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Actor_State_Com::SetDamage(float p_atk)
{
	if ( m_ISDie )
		return;

	int tempatk = p_atk;
	HP -= tempatk;

	SetHP(HP);
}

void Actor_State_Com::SetHP(int p_hp)
{
	if ( m_ISDie )
		return;

	HP = p_hp;
	if ( HP <= 0 )
	{
		m_ISDie = true;
	}
}

void Actor_State_Com::InitSettings( )
{
	// 컴포넌트들 추가



}

void Actor_State_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
