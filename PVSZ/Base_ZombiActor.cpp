#include "Base_ZombiActor.h"
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

#include "InGameDefineDatas.h"
#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"


using namespace std;

Base_ZombiActor* Base_ZombiActor::Create_ZombiActorObject( )
{

	return nullptr;
}

Base_ZombiActor::Base_ZombiActor( )
{
	SetZombiStateData(2, 50);
}

Base_ZombiActor::~Base_ZombiActor( )
{

}

void Base_ZombiActor::SetZombiStateData(int hp, int speed)
{
	m_ZombiStateData.HP = hp;
	m_ZombiStateData.Speed = speed;
}

ZombiStateData& Base_ZombiActor::GetZombiStateData( )
{
	return m_ZombiStateData;
}

void Base_ZombiActor::Start()
{
	__super::Start();

}

void Base_ZombiActor::Update(float dt)
{
	__super::Update(dt);



}

void Base_ZombiActor::Render(HDC hdc)
{
	__super::Render(hdc);


}


void Base_ZombiActor::InitSettings( )
{
	
}

void Base_ZombiActor::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
