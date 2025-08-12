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

#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"

using namespace std;

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

void Pea_Com::InitSettings( )
{
	
}

void Pea_Com::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}
