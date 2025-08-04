#include "Test_PlayerMoveCom.h"

#include "../Core/InputManager.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Compoment/Collider.h"
#include "../Compoment/RigidbodyComponent.h"
#include "../Core/UtilLoger.h"
#include "../Core/MyUtil.h"


Test_PlayerMoveCom::Test_PlayerMoveCom()
{
}

void Test_PlayerMoveCom::Initialize_AddCompoment()
{
	__super::Initialize_AddCompoment();


	m_LinkRigidBodyCom = owner->GetComponent<RigidbodyComponent>();
}

void Test_PlayerMoveCom::RemoveCompoment()
{
}

void Test_PlayerMoveCom::Start()
{
	__super::Start();
}

void Test_PlayerMoveCom::Update(float dt)
{
	__super::Update(dt);

	
	float movex = 0.f;
	float movey = 0.f;
	if (InputManager::IsKeyPressed(InputKey::A))
	{
		movex = -1.f;
	}
	if (InputManager::IsKeyPressed(InputKey::D))
	{
		movex = 1.f;
	}
	if (InputManager::IsKeyPressed(InputKey::W))
	{
		movey = -1.f;
	}
	if (InputManager::IsKeyPressed(InputKey::S))
	{
		movey = +1.f;
	}

	transform->SetTranslate(m_MoveSpeed * movex * dt, m_MoveSpeed * movey * dt);


	if (InputManager::ISKeyDown(InputKey::Space))
	{
		m_LinkRigidBodyCom->SetGravity(m_GrivityVal);
		m_LinkRigidBodyCom->AddForce(0, m_AddJumpPower);
	}
}

//void Test_PlayerMoveCom::Render(HDC hdc)
//{
//	__super::Render(hdc);
//
//}
