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

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"

#include "InGameDefineDatas.h"

using namespace std;

Plant_PeaShooter* Plant_PeaShooter::Create_PeaShooterObject( )
{
	GameObject* peashooterobj = InGame_PVSZ::AddGameObject("PeaShooter");
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

void Plant_PeaShooter::InitSettings( )
{
	
}

void Plant_PeaShooter::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
