#include "RigidbodyComponent.h"
#include <functional>
#include <memory>

#include "../Core/GameObject.h"
#include "../Compoment/Camera.h"
#include "../Compoment/Collider.h"
#include "../Manager/CameraManager.h"
#include "../Manager/PhysicsManager.h"
#include "../Core/UtilLoger.h"



long RigidbodyComponent::nextId = 0; // static 멤버 초기화


void RigidbodyComponent::AddForce(PointF p_power)
{
	if ( grounded )
	{
		velocity.x = p_power.X;
		velocity.y = p_power.Y;
	}
	else
	{
		velocity.x += p_power.X;
		velocity.y += p_power.Y;
	}

	grounded = false;
}

void RigidbodyComponent::AddForce(float p_x, float p_y)
{
	if ( grounded )
	{
		velocity.x = p_x;
		velocity.y = p_y;
	}
	else
	{
		velocity.x += p_x;
		velocity.y += p_y;
	}

	grounded = false;
}

void RigidbodyComponent::SetVelocity(float p_x, float p_y)
{
	velocity.Set(p_x, p_y);

	grounded = false;
}

RigidbodyComponent::RigidbodyComponent( )
{
	id = ++nextId; // 고유 ID 할당

}

void RigidbodyComponent::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	m_PhysicManager = PhysicsManager::GetI( );
	m_PhysicManager->AddRigidbodyCom(this);

	SetLinkCollider(this->owner->GetComponent<Collider>( ));

}

void RigidbodyComponent::RemoveCompoment( )
{
	Collider* collider = this->owner->GetComponent<Collider>( );
	if ( collider )
	{
		//collider->SetLinkCollider(nullptr);
		collider->ResetRigidBody( );
	}

	SetLinkCollider(nullptr);

	//m_PhysicManager = PhysicsManager::GetI( );
	//m_PhysicManager->AddRigidbodyCom(this);

	m_PhysicManager = nullptr;
	PhysicsManager::GetI( )->RemoveRigidbodyCom(this);
}

void RigidbodyComponent::SetLinkCollider(Collider* p_collider)
{
	if ( m_LinkCollider )
	{
		m_LinkCollider->SetRigidBodyCallFN(
			nullptr,
			nullptr,
			nullptr );
		m_LinkCollider = nullptr;
	}

	m_LinkCollider = p_collider;

	if ( m_LinkCollider )
	{
		m_LinkCollider->SetRigidBodyCallFN(
			std::bind(&RigidbodyComponent::OnCollisionEnter, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&RigidbodyComponent::OnCollisionStay, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&RigidbodyComponent::OnCollisionExit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
		);
	}

}

void RigidbodyComponent::Start()
{

}

//void RigidbodyComponent::Update(float dt)
//{
//	
//
//
//}

void RigidbodyComponent::Render(HDC hdc)
{
    if (enabled == false) return;

    if (!transform) return;

    Gdiplus::Graphics graphics(hdc);


}

void RigidbodyComponent::PhysicsUpdate(float dt)
{
	const Vec2& gravity = m_PhysicManager->GetGravity( );

	float dtangular = m_AngularDrag * dt;


	if ( velocity.LengthSquared( ) >= 0.0001f )
	{

		Vec2 normal = velocity.Normalize( );

		// 저항 처리
		//m_CalcPointF
		float dtlinear = m_LinearDrag * dt;
		m_CalcPointF.x = -normal.x * dtlinear;
		m_CalcPointF.y = -normal.y * dtlinear;

		velocity.x += m_CalcPointF.x;// +( gravity.X * dt );
		velocity.y += m_CalcPointF.y;
	}


	// 그라운드에 있으면 무조건 멈춤 적용
	if ( !grounded )
	{
		// gravity 값 적용하기
		//if ( velocity.x < gravity.x )
		velocity.x += ( gravity.x * dt ) * m_Gravity;

		//if ( velocity.y < gravity.y )
		velocity.y += ( gravity.y * dt ) * m_Gravity;

		transform->SetTranslate(velocity.x * dt, -velocity.y * dt);
	}
}

void RigidbodyComponent::OnCollisionEnter(Collider* p_src, Collider* p_other, void* p_data)
{
	UtilLoger::Log(L"강체진입 콜백 호출 : ");

	//grounded = true;


	//UpdateReLocation(p_src, p_other);


	UpdateResoveCollision(p_other);
	

}

void RigidbodyComponent::OnCollisionStay(Collider* p_src, Collider* p_other, void* p_data)
{
	//UtilLoger::Log(L"강체업데이트 콜백 호출 : ");


	//// 이부분에서 속도를 강제로 줄일지는 생각해야지됨
	//// enter 에서 처리할지 생각하기
	//velocity.Set(0.f, 0.f);
	//grounded = true;

	UpdateResoveCollision(p_other);
}

void RigidbodyComponent::OnCollisionExit(Collider* p_src, Collider* p_other, void* p_data)
{
	UtilLoger::Log(L"강체빠짐 콜백 호출 : ");
	
	grounded = false;
}

void RigidbodyComponent::UpdateReLocation(Collider* p_src, Collider* p_other)
{
	auto thisrect = p_src->GetWorldRect( );

	// 위치 다시 지정
	auto otherrect = p_other->GetWorldRect( );// GetAABB( );

	float yy = 0.f;
	// -쪽이 위에 있음
	if ( thisrect.Y < otherrect.Y )
	{
		//float top = rect.Y - rect.Height * 0.5f;
		float top = otherrect.GetTop( );
		yy = top - ( thisrect.Height );
	}
	else
	{
		float bottom = otherrect.GetBottom( );
		yy = bottom + ( thisrect.Height );
	}

	transform->SetWorldPosition(thisrect.X, yy);


	//Gdiplus::LinearGradientBrush
	//LineIntersect()
}

void RigidbodyComponent::UpdateResoveCollision( Collider* p_other )
{
	if ( !this->m_LinkCollider )
	{
		return;
	}

	Vec2 wpos = transform->GetWorldPosition( );
	Vec2 otherwpos = p_other->transform->GetWorldPosition( );


	SizeF this_size(this->m_LinkCollider->width, this->m_LinkCollider->height);
	SizeF other_size(p_other->width, p_other->height);

	// 일반 충돌 처리
	float bottomDist = fabs(( wpos.y + this_size.Height ) - otherwpos.y);
	float topDist = fabs(wpos.y - ( otherwpos.y + other_size.Height ));
	float leftDist = fabs(( wpos.x + this_size.Width ) - otherwpos.x);
	float rightDist = fabs(wpos.x - ( otherwpos.x + other_size.Width ));

	Vec2 temppos(wpos);
	bool ishit = false;
	// 상단
	if ( bottomDist < topDist && bottomDist < leftDist && bottomDist < rightDist ) {
		temppos.y = otherwpos.y - this_size.Height;
		velocity.y = 0;
		grounded = true;
		ishit = true;
	}
	// 하단
	else if ( topDist < bottomDist && topDist < leftDist && topDist < rightDist ) {
		temppos.y = otherwpos.y + this_size.Height;
		velocity.y *= -0.4f;
		ishit = true;
	}
	// 왼쪽
	else if ( leftDist < rightDist ) {
		temppos.x = otherwpos.x - this_size.Width;
		velocity.x *= -0.5f;
		ishit = true;
	}
	// 오른쪽
	else {
		temppos.x = otherwpos.x + other_size.Width;
		velocity.x *= -0.5f;
		ishit = true;
	}


	if ( ishit)
	{
		transform->SetWorldPosition(temppos.x, temppos.y);
	}
}

