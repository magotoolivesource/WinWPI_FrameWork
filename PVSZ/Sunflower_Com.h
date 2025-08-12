#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include <vector>


class UI_SelectBTNCom;
class Button;
class UtilTimer;
class Sunflower_Com;

class Sunflower_Com : public Component
{
public:
	static Sunflower_Com* CreateSunflower_Com( );

public:
	Sunflower_Com();
	virtual ~Sunflower_Com( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;

protected:
	float m_DelayShot = 3.f;
	float m_AttackVal = 1.f;

	float m_BuyCoin = 50.f;


protected:
	//void BaseGame::Test_AniCallBackFN1(UtilTimer* utiltimer, void* p_data)
	void Call_CreateSunFN(UtilTimer* utiltimer, void* p_data);
};

