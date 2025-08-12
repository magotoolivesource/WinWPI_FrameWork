#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include <vector>
#include <Core/DefineHeader.h>


class UI_SelectBTNCom;
class Button;

class Actor_State_Com : public Component
{
public:
	Actor_State_Com();
	virtual ~Actor_State_Com( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;



public:
	void SetDamage(float p_atk);
	void SetHP(int p_hp);

protected:
	int HP;
	GETSETER(bool, ISDie, false);

protected:
	void UpdateTimer( );
};

