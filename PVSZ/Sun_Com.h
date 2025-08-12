#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include <vector>


class UI_SelectBTNCom;
class Button;
class BoxCollider;


class Sun_Com : public Component
{
public:
	static Sun_Com* Create_SunObject( );

public:
	Sun_Com();
	virtual ~Sun_Com( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;


protected:
	void UpdateTimer( );

protected:
	BoxCollider* m_LinkBoxCollider = nullptr;
};

