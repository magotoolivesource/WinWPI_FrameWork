#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX

#include "Component.h"
//#include "Transform.h"
#include "../Core/Vector.h"
#include "../Core/Rect.h"
#include "../Core/DefineHeader.h"


using namespace Gdiplus;
class Transform;
class PhysicsManager;
class Collider;


// https://chatgpt.com/share/688b00f2-ab48-8013-9972-76d66264920a
class RigidbodyComponent : public Component
{
	
public:
	static long nextId; // 고유 ID 생성을 위함
	long id; // 충돌 감지를 위한 고유 ID

protected:
	PhysicsManager* m_PhysicManager;
	Collider* m_LinkCollider = nullptr;

protected:
	Vec2 m_CalcPointF;
	//float LinearDrag = 0.f;
	//float AngularDrag = 0.005f;
	Vec2 velocity;
	Vec2 delta_velocity;

	Vec2 PrevPos;

	bool grounded = false;

	float Friction = 0.f; // 마찰계수
	float Bouniness = 0.f; // 바운드값

	GETSETER(float, AngularDrag, 0.005f);
	GETSETER(float, LinearDrag, 0.0f);
	// 중력값처리용
	GETSETER(float, Gravity, 1.f);
	//float m_AngularDrag = 0.005f;
	//float GetAngularDrag( ) { return m_AngularDrag; }
	//void SetAngularDrag( float p_val ) { m_AngularDrag = p_val; }
	

public:
	void SetLinkCollider(Collider* p_collider);

	virtual void AddForce(PointF p_power);
	virtual void AddForce(float p_x, float p_y);

	virtual void SetVelocity(float p_x, float p_y);

	//void ApplyGravity(float gravity) {
	//	if ( !grounded ) {
	//		velocity.y += gravity;
	//	}
	//}

	//// 충돌 체크(단순 AABB)
	//bool CheckCollision(const RigidbodyComponent& other) {
	//	return !( position.x + size.x < other.position.x ||
	//		position.x > other.position.x + other.size.x ||
	//		position.y + size.y < other.position.y ||
	//		position.y > other.position.y + other.size.y );
	//}

public:
	RigidbodyComponent();
	virtual void Initialize_AddCompoment( ) override;
	virtual void RemoveCompoment( ) override;

    virtual ~RigidbodyComponent() = default;


    virtual void Start() override;
	// 컴포넌트에서 업데이트 하기 위한것
    //virtual void Update(float dt) override;
    virtual void Render(HDC hdc) override;

	// 매니저에서 직접 업데이트 처리
	virtual void PhysicsUpdate(float dt);
public:
	// ICollisionEventListener 인터페이스 구현 (기본 구현)
	void OnCollisionEnter(Collider* p_src, Collider* p_dest, void* p_data);
	void OnCollisionStay(Collider* p_src, Collider* p_dest, void* p_data);
	void OnCollisionExit(Collider* p_src, Collider* p_dest, void* p_data);
};

