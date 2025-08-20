#include "Move_Com.h"
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

Move_Com::Move_Com( )
{
}

Move_Com::~Move_Com( )
{

}

void Move_Com::Start()
{
	__super::Start();

}

void Move_Com::Update(float dt)
{
	__super::Update(dt);


	//Vec2 temppos = transform->GetWorldPosition( );
	//temppos += m_MoveSpeed * dt;

	Vec2 tempdeltamove = m_MoveSpeed* dt;
	transform->SetTranslate(tempdeltamove.x, tempdeltamove.y);



	// 화면 밖으로 나가면 삭제 적용
	Vec2 temppos = transform->GetWorldPosition( );

	if (temppos.x < 100 || temppos.x > SCREEN_W - 100 ||
		temppos.y < 100 || temppos.y > SCREEN_H + 100
		)
	{
		InGame_PVSZ::DestroyGameObject(this->owner);
		return;
	}

}

void Move_Com::Render(HDC hdc)
{
	__super::Render(hdc);


}

void Move_Com::SetMoveSpeed(float speed, float speedy)
{
	m_MoveSpeed.x = speed;
	m_MoveSpeed.y = speedy;
}

void Move_Com::SetMoveSpeed(Vec2 p_speed)
{
	m_MoveSpeed = p_speed;
}

void Move_Com::InitSettings( )
{
	
}

void Move_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
