#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>
#include "Base_ZombiActor.h"

#include "ZombieFactoryManager.h""

//class UI_SelectBTNCom;
//class Button;

class NormalZombie;


class NormalZombie : public AutoZombie<NormalZombie>
{
public:
	NormalZombie();
	virtual ~NormalZombie( ) override;

public:
	void InitSettings( ) override;
	void Initialize_AddCompoment( ) override;

	void InitZombiSetting( );

public:
	void Start( ) override;
	void Update(float dt) override;
	void Render(HDC hdc) override;


};

