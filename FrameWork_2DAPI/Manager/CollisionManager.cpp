#include "CollisionManager.h"
#include <windows.h>
#include <gdiplus.h>
#include <limits>
#include "../Core/DefineHeader.h"


bool CollisionManager::ISBoxCollider(BoxCollider* p_box, POINT p_mousepos)
{
	RectF rectf = p_box->GetAABB( );

	POINT mouse = p_mousepos;
	bool inside = mouse.x >= rectf.X && mouse.x <= rectf.X + rectf.Width &&
		mouse.y >= rectf.Y && mouse.y <= rectf.Y + rectf.Height;


	return inside;
}

CollisionManager::~CollisionManager()
{ 
	delete quadtree;
    for (Collider* c : colliders) {
        delete c; // 관리자가 충돌체들을 소유하고 있다면 해제
    }
}

void CollisionManager::Initialize() 
{ 
	//RectF worldBounds;
	//quadtree = new QuadtreeNode(worldBounds, 0);


}

void CollisionManager::CreateInitializeManager( )
{
	//if ( m_ISInit )
	//	return;
	//__super::CreateInitializeManager( );

	RectF worldBounds;
	quadtree = new QuadtreeNode(worldBounds, 0);
}

void CollisionManager::DrawColliders(HDC p_hdc)
{
	if ( !m_ISDebugDraw )
		return;

	Gdiplus::Graphics graphics(p_hdc);
	graphics.ResetTransform( );

	DrawColliders(&graphics);

}

void CollisionManager::DrawColliders(Graphics* g)
{
	if ( !m_ISDebugDraw )
		return;

	Pen greenPen(Color(255, 0, 255, 0), 2); // Green (No collision)
	Pen redPen(Color(255, 255, 0, 255), 2); // Red (Collision)
	Pen quadtreePen(Color(255, 128, 128, 128), 1); // Gray for quadtree

	// 쿼드트리 그리기 (디버깅용)
	// quadtree->Draw(g, &quadtreePen);
	quadtree->Draw(g, &quadtreePen);

	for ( Collider* c : colliders ) {
		Pen* currentPen = &greenPen;
		if ( !c->currentCollisions.empty( ) ) { // 현재 충돌 중인 것이 있으면
			currentPen = &redPen;
		}

		BoxCollider* box = dynamic_cast< BoxCollider* >( c );
		if ( box ) {
			std::vector<PointF> corners = box->GetWorldCornersF( );

			g->DrawLine(currentPen, corners[ 0 ], corners[ 1 ]);
			g->DrawLine(currentPen, corners[ 1 ], corners[ 2 ]);
			g->DrawLine(currentPen, corners[ 2 ], corners[ 3 ]);
			g->DrawLine(currentPen, corners[ 3 ], corners[ 0 ]);
		}
		else
		{
			//CircleCollider* circle = dynamic_cast<CircleCollider*>(c);
			//if (circle) {
			//    float worldRadius = circle->GetWorldRadius();
			//    graphics.DrawEllipse(
			//        currentPen, circle->position.X - worldRadius, circle->position.Y - worldRadius, worldRadius * 2, worldRadius * 2);
			//}
		}
	}
}

Component* CollisionManager::Raycast(const PointF& rayStart, const PointF& rayEnd
	, PointF& outHitPoint) 
{
	Component* closestHit = nullptr;
	float closestDistSq = FLT_MAX;

	for ( auto& entry : colliders ) {
		PointF hit;
		bool hitDetected = false;

		if ( entry->GetColliderType() == ColliderType::Box ) {
			auto box = static_cast< BoxCollider* >( entry );
			hitDetected = RaycastBox(*box, rayStart, rayEnd, hit);
		}
		else if ( entry->GetColliderType() == ColliderType::Circle ) {
			auto circle = static_cast< CircleCollider* >( entry );
			hitDetected = RaycastCircle(*circle, rayStart, rayEnd, hit);
		}

		if ( hitDetected ) {
			float dx = hit.X - rayStart.X;
			float dy = hit.Y - rayStart.Y;
			float distSq = dx * dx + dy * dy;
			if ( distSq < closestDistSq ) {
				closestDistSq = distSq;
				outHitPoint = hit;
				closestHit = entry;
			}
		}
	}

	return closestHit;
}

std::tuple<Component*, Gdiplus::PointF, bool> CollisionManager::RaycastTuple(const PointF& rayStart, const PointF& rayEnd)
{
	Component* closestHit = nullptr;
	Gdiplus::PointF outHitPoint(0, 0);
	float closestDistSq = FLT_MAX;
	bool ishit = false;
	for ( auto& entry : colliders ) {
		PointF hit;
		bool hitDetected = false;

		if ( entry->GetColliderType( ) == ColliderType::Box ) {
			auto box = static_cast< BoxCollider* >( entry );
			hitDetected = RaycastBox(*box, rayStart, rayEnd, hit);
		}
		else if ( entry->GetColliderType( ) == ColliderType::Circle ) {
			auto circle = static_cast< CircleCollider* >( entry );
			hitDetected = RaycastCircle(*circle, rayStart, rayEnd, hit);
		}

		if ( hitDetected ) {
			float dx = hit.X - rayStart.X;
			float dy = hit.Y - rayStart.Y;
			float distSq = dx * dx + dy * dy;
			if ( distSq < closestDistSq ) {
				closestDistSq = distSq;
				outHitPoint = hit;
				closestHit = entry;
				ishit = true;
			}
		}
	}

	return { closestHit, outHitPoint, ishit };
}

bool CollisionManager::RaycastBox(const BoxCollider& box
	, const PointF& rayStart
	, const PointF& rayEnd
	, PointF& outHitPoint) {
	std::vector<PointF> corners = GetRotatedCorners(box);
	for ( int i = 0; i < 4; ++i ) {
		PointF p1 = corners[ i ];
		PointF p2 = corners[ ( i + 1 ) % 4 ];
		if ( LineIntersect(rayStart, rayEnd, p1, p2, outHitPoint) )
			return true;
	}
	return false;
}

std::vector<PointF> CollisionManager::GetRotatedCorners(const BoxCollider& box) {
	std::vector<PointF> corners(4);

	float rad = box.rotation * DEG2RAD;// 3.14159f / 180.0f;
	float cosA = cos(rad), sinA = sin(rad);

	float hw = box.width * 0.5f;
	float hh = box.height * 0.5f;
	PointF local[ 4 ] = {
		{-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh}
	};

	corners.clear( );
	box.GetWorldCornersF( &corners);

	////RectF rectf = box.GetAABB( );
	//Vec2 pos = box.transform->GetWorldPosition( );
	//for ( int i = 0; i < 4; ++i ) {
	//	//corners[ i ].X = box.center.x + static_cast< int >(local[ i ].X * cosA - local[ i ].Y * sinA);
	//	//corners[ i ].Y = box.center.y + static_cast< int >(local[ i ].X * sinA + local[ i ].Y * cosA);
	//	corners[ i ].X = pos.x + static_cast< int >(local[ i ].X * cosA - local[ i ].Y * sinA);
	//	corners[ i ].Y = pos.y + static_cast< int >(local[ i ].X * sinA + local[ i ].Y * cosA);
	//}

	return corners;
}

bool CollisionManager::RaycastCircle(const CircleCollider& circle
	, const PointF& rayStart
	, const PointF& rayEnd
	, PointF& outHitPoint) 
{

	Vec2 pos = circle.transform->GetWorldPosition( );
	PointF dir = { rayEnd.X - rayStart.X, rayEnd.Y - rayStart.Y };
	PointF f = { rayStart.X - pos.x, rayStart.Y - pos.y };

	float a = dir.X * dir.X + dir.Y * dir.Y;
	float b = 2 * ( f.X * dir.X + f.Y * dir.Y );
	float c = f.X * f.X + f.Y * f.Y - circle.m_Radius * circle.m_Radius;

	float discriminant = b * b - 4 * a * c;

	if ( discriminant < 0 )
		return false;

	discriminant = sqrt(discriminant);
	float t1 = ( -b - discriminant ) / ( 2 * a );
	float t2 = ( -b + discriminant ) / ( 2 * a );

	if ( t1 >= 0.0f && t1 <= 1.0f ) {
		outHitPoint.X = static_cast< int >(rayStart.X + dir.X * t1);
		outHitPoint.Y = static_cast< int >(rayStart.Y + dir.Y * t1);
		return true;
	}
	else if ( t2 >= 0.0f && t2 <= 1.0f ) {
		outHitPoint.X = static_cast< int >( rayStart.X + dir.X * t2 );
		outHitPoint.Y = static_cast< int >( rayStart.Y + dir.Y * t2 );
		return true;
	}

	return false;
}

bool CollisionManager::LineIntersect(const PointF& a1
	, const PointF& a2
	, const PointF& b1
	, const PointF& b2
	, PointF& intersection)
{
	

	float x1 = ( float ) a1.X, y1 = ( float ) a1.Y;
	float x2 = ( float ) a2.X, y2 = ( float ) a2.Y;
	float x3 = ( float ) b1.X, y3 = ( float ) b1.Y;
	float x4 = ( float ) b2.X, y4 = ( float ) b2.Y;

	float denom = ( x1 - x2 ) * ( y3 - y4 ) - ( y1 - y2 ) * ( x3 - x4 );
	//if ( fabs(denom) == 0.0f ) return false;
	if ( fabs(denom) < std::numeric_limits<float>::epsilon( ) )
	{
		return false;
	}


	float t = ( ( x1 - x3 ) * ( y3 - y4 ) - ( y1 - y3 ) * ( x3 - x4 ) ) / denom;
	float u = ( ( x1 - x3 ) * ( y1 - y2 ) - ( y1 - y3 ) * ( x1 - x2 ) ) / denom;

	if ( t >= 0 && t <= 1 && u >= 0 && u <= 1 ) {
		intersection.X = ( LONG ) ( x1 + t * ( x2 - x1 ) );
		intersection.Y = ( LONG ) ( y1 + t * ( y2 - y1 ) );
		return true;
	}
	return false;
}

