#include "CollisionManager.h"
#include <windows.h>
#include <gdiplus.h>

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
	if ( m_ISInit )
		return;
	__super::CreateInitializeManager( );

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
