#pragma once
#include "../Core/Rect.h"
#include "Component.h"
#include "Transform.h"

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <limits> // For FLT_MAX
#include "../Core/Vector.h"



// https://gemini.google.com/app/92e317d273e6a9b5
// GDI+ 네임스페이스 사용
using namespace Gdiplus;

// Vec2는 Gdiplus::PointF로 대체됩니다.
// LineSegment는 Gdiplus::PointF를 사용하여 정의됩니다.
struct LineSegment {
    Vec2 p1, p2;
};


class BoxCollider;


// --- 충돌 감지 구현 ---

// 사각형-사각형 충돌을 위한 분리축 정리 (SAT)
bool CheckBoxBoxCollision(BoxCollider* box1, BoxCollider* box2);
// 선-선 교차를 위한 헬퍼 (사각형-선에 사용됨)
// 세그먼트가 교차하면 true 반환
bool LineLineIntersection(const LineSegment& line1, const LineSegment& line2);


class Collider : public Component 
{
public:
    Collider(Vec2 pos, float rot, SizeF s)
	{ 
		transform->SetWorldPosition(pos.x, pos.y);
        transform->SetWorldRotation(rot);
        transform->SetWorldScale(s.Width, s.Height);
	}
    virtual ~Collider() { }

protected:
    Gdiplus::Matrix m_TempCalcMatrix;
    float m_tempcachedmatrix[(int)MatrixElements::Size] = { 0, 0, 0, 0, 0, 0 };


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
};



// --- Box Collider ---
class BoxCollider : public Collider {
public:
    float width, height; // Local dimensions (before scale)

    BoxCollider(Vec2 pos, float rot, SizeF s, float w, float h)
        : Collider(pos, rot, s)
        , width(w)
        , height(h) 
	{ 
		
	}

	virtual ~BoxCollider() { }

	// Get the corners of the box in world space using GDI+ Matrix
    std::vector<Vec2> GetWorldCorners() const;


	// Collision dispatch
    bool CollidesWith(Collider* other) { return other->CollidesWithBox(this); }
    bool CollidesWithBox(BoxCollider* other) { return CheckBoxBoxCollision(this, other); }
    //bool CollidesWithCircle(CircleCollider* circle) { return CheckBoxCircleCollision(this, circle); }

	// Point collision (using InverseTransformPoint)
    bool CollidesWith(const Vec2& point) override {
        Vec2 localPoint = InverseTransformPoint(point);
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
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

};
