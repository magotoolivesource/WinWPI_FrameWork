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
class Collider;
class UtilTimer;

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

protected:
	virtual void Collider_TriggerEnter(Collider* src_other, Collider* dest_other, void* p_data) override;
	virtual void Collider_TriggerStay(Collider* src_other, Collider* dest_other, void* p_data) override;
	virtual void Collider_TriggerExit(Collider* src_other, Collider* dest_other, void* p_data) override;

public:
	virtual void SetDieEvent( ) override;
	virtual void SetDamage(float p_atk) override;
	virtual void UpdateUI( ) override;
	virtual void SetMove(bool p_ismove) override;

	virtual void DestroyEvent( ) override;
	virtual void DestroyEvent_CallFN(UtilTimer* p_timer, void* p_data);

protected:
	void InitSettingBaseAnimations( );
	void Set_DieAnimations( );
};

