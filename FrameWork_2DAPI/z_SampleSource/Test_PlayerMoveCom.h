#pragma once

#include "../Compoment/Component.h"


class Collider;
class RigidbodyComponent;


class Test_PlayerMoveCom : public Component
{

protected:
	Collider* m_LinkCollider;
	RigidbodyComponent* m_LinkRigidBodyCom;

	float m_GrivityVal = 35.f;
	float m_AddJumpPower = 300.f;

	float m_MoveSpeed = 50.f;

public:
	Test_PlayerMoveCom( );
	virtual void Initialize_AddCompoment( ) override;
	virtual void RemoveCompoment( ) override;

	virtual ~Test_PlayerMoveCom( ) = default;


	virtual void Start( ) override;
	virtual void Update(float dt) override;
	//virtual void Render(HDC hdc) override;
};

