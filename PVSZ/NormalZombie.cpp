#include "NormalZombie.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <UICompoment/Button.h>
#include <Core/UtilLoger.h>

#include "InGameDefineDatas.h"
#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"


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
}
