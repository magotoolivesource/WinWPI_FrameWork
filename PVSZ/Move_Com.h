#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>


class UI_SelectBTNCom;
class Button;

class Move_Com : public Component
{
public:
	Move_Com();
	virtual ~Move_Com( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;


protected:
	Vec2 m_MoveSpeed = { 0.f, 0.f }; // 이동 속도 벡터

public:
	//void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
	void SetMoveSpeed(float speed, float speedy);
	void SetMoveSpeed(Vec2 p_speed);

};

