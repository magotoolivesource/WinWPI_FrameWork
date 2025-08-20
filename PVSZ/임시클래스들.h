#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>


class UI_SelectBTNCom;
class Button;

class Base_ZombiActor : public Component
{
public:
	Base_ZombiActor();
	virtual ~Base_ZombiActor( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;



};

