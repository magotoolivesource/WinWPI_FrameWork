//#pragma once
#include "Collider.h"
#include "../Core/GameObject.h"
#include "Component.h"
#include "Transform.h"

#include "../Manager/CollisionManager.h"
#include "../Core/UtilLoger.h"
#include "RigidbodyComponent.h"

using namespace Gdiplus;



long Collider::nextId = 0; // static 멤버 초기화


// --- 충돌 감지 구현 ---

// 사각형-사각형 충돌을 위한 분리축 정리 (SAT)
bool CheckBoxBoxCollision(BoxCollider* box1, BoxCollider* box2) {
    // 단순화된 SAT 구현입니다. 완전한 SAT는 모든 축에 모든 모서리를 투영하고
    // 겹침을 확인해야 합니다. 사각형의 경우, 축은 단순히 모서리의 법선입니다.

    // 월드 모서리 가져오기
    std::vector<Vec2> corners1 = box1->GetWorldCorners();
    std::vector<Vec2> corners2 = box2->GetWorldCorners();


    // 확인할 축: 두 사각형 모서리의 법선
    std::vector<Vec2> axes;
    axes.push_back((corners1[1] - corners1[0]).Normalize().Perpendicular());
    axes.push_back((corners1[2] - corners1[1]).Normalize().Perpendicular());
    axes.push_back((corners2[1] - corners2[0]).Normalize().Perpendicular());
    axes.push_back((corners2[2] - corners2[1]).Normalize().Perpendicular());

    for (const auto& axis : axes) {
        float min1 = FLT_MAX, max1 = -FLT_MAX;
        for (const auto& p : corners1) {
            float projection = p.Dot(axis);
            if (projection < min1) min1 = projection;
            if (projection > max1) max1 = projection;
        }

        float min2 = FLT_MAX, max2 = -FLT_MAX;
        for (const auto& p : corners2) {
            float projection = p.Dot(axis);
            if (projection < min2) min2 = projection;
            if (projection > max2) max2 = projection;
        }

        // 이 축에서 겹침이 없으면 사각형은 충돌하지 않습니다.
        if (!(max1 >= min2 && max2 >= min1)) {
            return false;
        }
    }
    return true; // 모든 축에서 겹침, 충돌 중
}

// 선-선 교차를 위한 헬퍼 (사각형-선에 사용됨)
// 세그먼트가 교차하면 true 반환
bool LineLineIntersection(const LineSegment& line1, const LineSegment& line2) {
    Vec2 p1 = line1.p1, q1 = line1.p2;
    Vec2 p2 = line2.p1, q2 = line2.p2;


    float s1_x, s1_y, s2_x, s2_y;
    s1_x = q1.x - p1.x;
    s1_y = q1.y - p1.y;
    s2_x = q2.x - p2.x;
    s2_y = q2.y - p2.y;

    float denominator = (-s2_x * s1_y + s1_x * s2_y);
    if (std::abs(denominator) < 1e-6) // 선들이 평행
        return false;

    float s = (-s1_y * (p1.x - p2.x) + s1_x * (p1.y - p2.y)) / denominator;
    float t = (s2_x * (p1.y - p2.y) - s2_y * (p1.x - p2.x)) / denominator;

    return (s >= 0 && s <= 1 && t >= 0 && t <= 1);
}


void Collider::SetCallFN(Callback p_enter, Callback p_stay, Callback p_exit)
{
	m_EnterCallBack = p_enter;
	m_StayCallBack = p_stay;
	m_ExitCallBack = p_exit;
}

void Collider::SetRigidBodyCallFN(Callback p_enter, Callback p_stay, Callback p_exit)
{
	m_Rigidbody_EnterCallBack = p_enter;
	m_Rigidbody_StayCallBack = p_stay;
	m_Rigidbody_ExitCallBack = p_exit;
}

Collider::~Collider()
{ 
	CollisionManager::GetI()->RemoveCollider(this); 
}

void Collider::Initialize_AddCompoment() { 

	__super::Initialize_AddCompoment( );

	transform->SetWorldPosition(pos.x, pos.y);
    transform->SetWorldRotation(rotation);
    //transform->SetWorldScale(scale.Width, scale.Height);

	CollisionManager::GetI()->AddCollider(this); 


	m_Bodycom = this->owner->GetComponent<RigidbodyComponent>( );
	//RigidbodyComponent* bodycom = 
	if ( m_Bodycom )
	{
		m_Bodycom->SetLinkCollider(this);
	}
}

void Collider::RemoveCompoment( )
{
	RigidbodyComponent* bodycom = this->owner->GetComponent<RigidbodyComponent>( );
	if ( bodycom )
	{
		bodycom->SetLinkCollider(nullptr);
	}
}

bool Collider::Intersects(Collider* other) {
    Gdiplus::RectF a = GetWorldRect();
    Gdiplus::RectF b = other->GetWorldRect();

    return a.Intersect(b); //    a.Intersects(b);

}

void Collider::OnCollisionEnter(Collider* other) {
	// 실제 게임 로직에서 충돌 시작 시 필요한 동작을 여기에 추가
	// 예를 들어, DebugPrint("충돌 시작: " + other->id);
	std::wstring msg = std::format(L"{}, {} 충돌엔터", this->id, other->id);
	UtilLoger::Log(msg, E_LogLevel::E_INFO);

	if ( m_EnterCallBack )
		m_EnterCallBack(this, other, nullptr);

	if ( m_Rigidbody_EnterCallBack )
		m_Rigidbody_EnterCallBack(this, other, nullptr);

}

void Collider::OnCollisionStay(Collider* other) {
	// 실제 게임 로직에서 충돌 유지 시 필요한 동작을 여기에 추가
	// 예를 들어, DebugPrint("충돌 중: " + other->id);

	if ( m_StayCallBack )
		m_StayCallBack(this, other, nullptr);

	if ( m_Rigidbody_StayCallBack )
		m_Rigidbody_StayCallBack(this, other, nullptr);
}

void Collider::OnCollisionExit(Collider* other) {
	// 실제 게임 로직에서 충돌 종료 시 필요한 동작을 여기에 추가
	// 예를 들어, DebugPrint("충돌 나감: " + other->id);
	std::wstring msg = std::format(L"{}, {} 충돌빠지기", this->id, other->id);
	UtilLoger::Log(msg, E_LogLevel::E_INFO);

	if ( m_ExitCallBack )
		m_ExitCallBack(this, other, nullptr);

	if ( m_Rigidbody_ExitCallBack )
		m_Rigidbody_ExitCallBack(this, other, nullptr);
}

void Collider::ResetRigidBody( )
{
	m_Bodycom = nullptr;
}


Gdiplus::RectF Collider::GetWorldRect() {
    //Transform* t = owner->GetComponent<Transform>();

	Transform* t = this->transform;
    return Gdiplus::RectF { t->GetWorldPosition().x, t->GetWorldPosition().y
		, width //t->width
		, height //t->height 
	};


}

void BoxCollider::SetBoxCollider(Vec2 pos, float rot, SizeF s, float w, float h) 
{ 
	this->pos = pos;
    position.X = pos.x;
    position.Y = pos.y;
    rotation = rot;
    scale = s;

	width = w;
    height = h;


	transform->SetWorldPosition(pos.x, pos.y);
    transform->SetWorldRotation(rot);
    //transform->SetWorldScale(s.Width, s.Height);

}

void BoxCollider::Initialize_AddCompoment() 
{ 
	__super::Initialize_AddCompoment(); 


}

void BoxCollider::RemoveCompoment( )
{


	__super::RemoveCompoment( );
}

std::vector<Vec2> BoxCollider::GetWorldCorners() const {
    std::vector<Vec2> corners;
    // Local corners relative to its own origin (0,0)
    Vec2 halfSize = Vec2(width * 0.5f, height * 0.5f);
    PointF localCorners[4] = { PointF(-halfSize.x, -halfSize.y)
		, PointF(halfSize.x, -halfSize.y)
		, PointF(halfSize.x, halfSize.y),
        PointF(-halfSize.x, halfSize.y) };


    Matrix& transformMatrix = transform->GetWorldMatrix();
    transformMatrix.TransformPoints(localCorners, 4);

    for (int i = 0; i < 4; ++i) {
        corners.push_back({ localCorners[i].X, localCorners[i].Y });
    }
    return corners;
}

std::vector<PointF> BoxCollider::GetWorldCornersF() const 
{ 
	std::vector<PointF> corners;
    //PointF halfSize = PointF(width / 2.0f, height / 2.0f);
	PointF halfSize = PointF(width * 0.5f, height * 0.5f);
    PointF localCorners[4] = { PointF(-halfSize.X, -halfSize.Y), 
		PointF(halfSize.X, -halfSize.Y), 
		PointF(halfSize.X, halfSize.Y),
        PointF(-halfSize.X, halfSize.Y) };

    Matrix& transformMatrix = transform->GetWorldMatrix();
    transformMatrix.TransformPoints(localCorners, 4);

    for (int i = 0; i < 4; ++i) {
        corners.push_back(localCorners[i]);
    }
    return corners;
}

void BoxCollider::GetWorldCornersF(OUT std::vector<PointF>* p_outconner) const
{
	//std::vector<PointF> corners;
	//PointF halfSize = PointF(width / 2.0f, height / 2.0f);
	PointF halfSize = PointF(width * 0.5f, height * 0.5f);
	PointF localCorners[ 4 ] = { PointF(-halfSize.X, -halfSize.Y),
		PointF(halfSize.X, -halfSize.Y),
		PointF(halfSize.X, halfSize.Y),
		PointF(-halfSize.X, halfSize.Y) };

	Matrix& transformMatrix = transform->GetWorldMatrix( );
	transformMatrix.TransformPoints(localCorners, 4);

	p_outconner->clear( );
	for ( int i = 0; i < 4; ++i ) {
		p_outconner->push_back(localCorners[ i ]);
	}
	return;
}
