#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include <vector>


class UI_SelectBTNCom;
class Button;
class UtilTimer;


class Plant_PeaShooter : public Component
{
public:
	static Plant_PeaShooter* Create_PeaShooterObject( );

public:
	Plant_PeaShooter();
	virtual ~Plant_PeaShooter( ) override;

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
	void Call_CreatePeaFN(UtilTimer* utiltimer, void* p_data);
};

