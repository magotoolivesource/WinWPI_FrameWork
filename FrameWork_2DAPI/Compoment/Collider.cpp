//#pragma once
#include "Collider.h"
#include "../Core/GameObject.h"
#include "Component.h"
#include "Transform.h"


using namespace Gdiplus;



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



bool Collider::Intersects(Collider* other) {
    Gdiplus::RectF a = GetWorldRect();
    Gdiplus::RectF b = other->GetWorldRect();

    return a.Intersect(b); //    a.Intersects(b);


}

Gdiplus::RectF Collider::GetWorldRect() {
    //Transform* t = owner->GetComponent<Transform>();

	Transform* t = this->transform;
    return Gdiplus::RectF { t->GetWorldPosition().x, t->GetWorldPosition().y
		, t->width, t->height };


}

std::vector<Vec2> BoxCollider::GetWorldCorners( ) const
{
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