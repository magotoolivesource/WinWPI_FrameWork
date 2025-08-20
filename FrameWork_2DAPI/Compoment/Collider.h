#pragma once
#define _USE_MATH_DEFINES // For M_PI
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm> // For std::max, std::min
#include <functional>
#include <memory>
#include <algorithm>
#include <limits> // For FLT_MAX
#include <string>
#include <format>

#include "../Core/Rect.h"
#include "../Core/Vector.h"
#include "../Core/DefineHeader.h"

#include "Component.h"
#include "Transform.h"



// https://gemini.google.com/app/92e317d273e6a9b5
// GDI+ 네임스페이스 사용
using namespace Gdiplus;

// Vec2는 Gdiplus::PointF로 대체됩니다.
// LineSegment는 Gdiplus::PointF를 사용하여 정의됩니다.
struct LineSegment {
    Vec2 p1, p2;
};


// =====================================================================
// 전방 선언
class Collider; // Collider가 ICollisionEventListener를 상속받으므로 먼저 선언
class BoxCollider;
class RigidbodyComponent;


// https://chatgpt.com/c/6890731a-0500-8013-8bd7-89b0ebf2e618
enum class ColliderType {
	Box,
	Circle
};




// --- 충돌 감지 구현 ---

// 사각형-사각형 충돌을 위한 분리축 정리 (SAT)
bool CheckBoxBoxCollision(BoxCollider* box1, BoxCollider* box2);
// 선-선 교차를 위한 헬퍼 (사각형-선에 사용됨)
// 세그먼트가 교차하면 true 반환
bool LineLineIntersection(const LineSegment& line1, const LineSegment& line2);



// 충돌 이벤트 리스너 인터페이스
class ICollisionEventListener {
public:
    virtual void OnCollisionEnter(Collider* other) = 0;
    virtual void OnCollisionStay(Collider* other) = 0;
    virtual void OnCollisionExit(Collider* other) = 0;
};


class Collider : public Component, ICollisionEventListener
{
public:
	using Callback = std::function<void(Collider*, Collider*, void*)>;

	virtual ColliderType GetColliderType( ) = 0;

protected:
	Callback m_EnterCallBack = nullptr;
	Callback m_StayCallBack = nullptr;
	Callback m_ExitCallBack = nullptr;


protected:
	Callback m_Rigidbody_EnterCallBack = nullptr;
	Callback m_Rigidbody_StayCallBack = nullptr;
	Callback m_Rigidbody_ExitCallBack = nullptr;

	void SetRigidBodyCallFN(Callback p_enter, Callback p_stay, Callback p_exit);
	friend class RigidbodyComponent;

public:
	void SetCallFN(Callback p_enter, Callback p_stay, Callback p_exit);
	


protected:
	RigidbodyComponent* m_Bodycom;

public:
    static long nextId; // 고유 ID 생성을 위함
    long id; // 충돌 감지를 위한 고유 ID

	Vec2 pos;
    PointF position; // 월드 위치
    float rotation; // 회전 (라디안)
    SizeF scale; // 스케일 요소

    // 충돌 상태 추적 (현재 충돌 중인 다른 충돌체의 ID를 저장)
    std::set<long> currentCollisions;

public:
    Collider(Vec2 pos, float rot, Gdiplus::SizeF s)
	{ 
		this->pos = pos;
        position.X = pos.x;
        position.Y = pos.y;
        rotation = rot;
		scale = s;

		//transform->SetWorldPosition(pos.x, pos.y);
  //      transform->SetWorldRotation(rot);
  //      transform->SetWorldScale(s.Width, s.Height);

		id = ++nextId; // 고유 ID 할당
	}
    virtual ~Collider();

protected:
    Gdiplus::Matrix m_TempCalcMatrix;
    float m_tempcachedmatrix[(int)MatrixElements::Size] = { 0, 0, 0, 0, 0, 0 };


protected:
	float width = 50.0f;
	float height = 50.0f;
public:
	float getWidth( ) const { return width; }
	float getHeight( ) const { return height; }
	void setWidth(float w) { width = w; }
	void setHeight(float h) { height = h; }

public:
    virtual void Initialize_AddCompoment() override;
	virtual void RemoveCompoment( ) override;

public:
    // Pure virtual functions for type-specific collision
    virtual bool CollidesWith(Collider* other) = 0;
    virtual bool CollidesWith(const Vec2& point) = 0;
    virtual bool CollidesWith(const LineSegment& line) = 0;

	// 다형적 디스패치를 위한 헬퍼
    virtual bool CollidesWithBox(BoxCollider* box) = 0;
    //virtual bool CollidesWithCircle(CircleCollider* circle) = 0;

public:
    bool Intersects(Collider* other);
    Gdiplus::RectF GetWorldRect();


	// GDI+ Matrix를 사용하여 월드 점을 로컬 공간으로 역변환
    Vec2 InverseTransformPoint(const Vec2& point) {

		m_TempCalcMatrix.Reset();
        auto state = GetInverseTransformMatrix(&m_TempCalcMatrix);
        //Vec2 transformedPoint = point;

		PointF pos(point.x, point.y);
        m_TempCalcMatrix.TransformPoints(&pos, 1);
        return { pos.X, pos.Y };
    }

	// GDI+ Matrix를 사용하여 역변환 매트릭스 생성
    Gdiplus::Status GetInverseTransformMatrix(OUT Gdiplus::Matrix* p_matrix){

		transform->GetWorldMatrix().GetElements(m_tempcachedmatrix);
		return p_matrix->SetElements(
					m_tempcachedmatrix[0], m_tempcachedmatrix[1],
					m_tempcachedmatrix[2], m_tempcachedmatrix[3],
					m_tempcachedmatrix[4], m_tempcachedmatrix[5]
				);


        //Gdiplus::Matrix matrix;
        //// GDI+ Matrix를 사용한 역변환은 순서가 역순이어야 합니다.
        //// 먼저 역이동
        //matrix.Translate(-position.x, -position.y, MatrixOrderAppend);
        //// 다음 역회전
        //matrix.Rotate(-rotation * 180.0f / M_PI, MatrixOrderAppend);
        //// 마지막으로 역스케일 (0으로 나누는 것을 방지)
        //matrix.Scale(1.0f / (scale.Width != 0 ? scale.Width : 1.0f), 1.0f / (scale.Height != 0 ? scale.Height : 1.0f), MatrixOrderAppend);

        //return matrix;
    }

public:
    // 모든 충돌체가 구현해야 하는 가상 함수: AABB (Axis-Aligned Bounding Box) 가져오기
    // 쿼드트리에서 객체를 삽입하고 검색하는 데 사용됩니다.
    virtual RectF GetAABB() const = 0;

    // ICollisionEventListener 인터페이스 구현 (기본 구현)
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:
	void ResetRigidBody( );

	GETSETER(bool, ISOneWay, false);

};



// --- Box Collider ---
class BoxCollider : public Collider {
public:
	virtual ColliderType GetColliderType( ) {
		return ColliderType::Box;
	}

public:
    float width, height; // Local dimensions (before scale)

	BoxCollider(): Collider({ 0, 0 }, 0, {50, 50})
		, width(100)
		, height(100)
	{
	}
    BoxCollider(Vec2 pos, float rot, SizeF s, float w, float h)
        : Collider(pos, rot, s)
        , width(w)
        , height(h) 
	{ 
		
	}

	virtual ~BoxCollider() { }


public:
    void SetBoxCollider(Vec2 pos, float rot, SizeF s, float w, float h);
	virtual void Initialize_AddCompoment() override;
	virtual void RemoveCompoment( ) override;

public:
	// Get the corners of the box in world space using GDI+ Matrix
    std::vector<Vec2> GetWorldCorners() const;
    std::vector<PointF> GetWorldCornersF() const;

	void GetWorldCornersF( OUT std::vector<PointF>* p_outconner ) const;


	// Collision dispatch
    bool CollidesWith(Collider* other) { return other->CollidesWithBox(this); }
    bool CollidesWithBox(BoxCollider* other) { return CheckBoxBoxCollision(this, other); }
    //bool CollidesWithCircle(CircleCollider* circle) { return CheckBoxCircleCollision(this, circle); }

	// Point collision (using InverseTransformPoint)
    bool CollidesWith(const Vec2& point) override {
        Vec2 localPoint = InverseTransformPoint(point);
        float halfWidth = width * 0.5f; // width / 2.0f;
        float halfHeight = height * 0.5f; // height / 2.0f;
        return (localPoint.x >= -halfWidth && localPoint.x <= halfWidth && localPoint.y >= -halfHeight && localPoint.y <= halfHeight);
    }

    // Line collision
    bool CollidesWith(const LineSegment& line) override {
        if (CollidesWith(line.p1) || CollidesWith(line.p2)) {
            return true;
        }

        std::vector<Vec2> corners = GetWorldCorners();
        // Check intersections with box edges
        if (LineLineIntersection(line, { corners[0], corners[1] })) return true;
        if (LineLineIntersection(line, { corners[1], corners[2] })) return true;
        if (LineLineIntersection(line, { corners[2], corners[3] })) return true;
        if (LineLineIntersection(line, { corners[3], corners[0] })) return true;



        return false;
    }


	// AABB 구현: 회전된 사각형의 AABB를 계산
    RectF GetAABB() const override {
        std::vector<Vec2> corners = GetWorldCorners();

        float minX = FLT_MAX; // std::numeric_limits<float>::max();
        float minY = FLT_MAX; // std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float maxY = std::numeric_limits<float>::lowest();

        for (const auto& p : corners) {
            if (p.x < minX) minX = p.x;
            if (p.y < minY) minY = p.y;
            if (p.x > maxX) maxX = p.x;
            if (p.y > maxY) maxY = p.y;
        }
        return RectF(minX, minY, maxX - minX, maxY - minY);
    }
};


class CircleCollider : public Collider 
{
public:
	virtual ColliderType GetColliderType( ) {
		return ColliderType::Box;
	}


public:
	float m_Radius; // Local dimensions (before scale)

	CircleCollider( ) : Collider({ 0, 0 }, 0, { 50, 50 })
		, m_Radius(1.f)
	{
	}
	CircleCollider(Vec2 pos, float p_size)
		: Collider(pos, 0.f, {p_size, p_size})
		, m_Radius(p_size)
	{

	}

	virtual ~CircleCollider( ) {}

public:


};

