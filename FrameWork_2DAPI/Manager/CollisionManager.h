#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

#include "../Core/SingletonT.h"
#include "../Compoment/Collider.h"


// Forward declaration of Gdiplus::Image to avoid including the entire Gdiplus header
namespace Gdiplus{
    class Image;
};
using namespace std;
using namespace Gdiplus;


// https://chatgpt.com/c/6890731a-0500-8013-8bd7-89b0ebf2e618
struct ColliderEntry {
	Collider* comp;
	ColliderType type;
};


// =====================================================================
// 쿼드트리 노드
class QuadtreeNode {
public:
    static const int MAX_OBJECTS = 4; // 한 노드가 가질 수 있는 최대 객체 수
    static const int MAX_LEVELS = 8; // 최대 분할 레벨 (재귀 깊이)

    RectF bounds; // 이 노드가 커버하는 영역
    int level; // 현재 노드의 레벨 (깊이)
    std::vector<Collider*> objects; // 이 노드에 속한 객체들
    QuadtreeNode* nodes[4]; // 자식 노드 (0: 북서, 1: 북동, 2: 남서, 3: 남동)

    QuadtreeNode(RectF pBounds, int pLevel)
        : bounds(pBounds)
        , level(pLevel) {
        for (int i = 0; i < 4; ++i) {
            nodes[i] = nullptr;
        }
    }

    ~QuadtreeNode() {
        Clear(); // 자식 노드 및 객체들을 해제
    }

    // 노드를 비움 (자식 노드 및 객체 참조 해제)
    void Clear() {
        objects.clear();
        for (int i = 0; i < 4; ++i) {
            if (nodes[i] != nullptr) {
                delete nodes[i];
                nodes[i] = nullptr;
            }
        }
    }

    // 노드를 4개의 자식 노드로 분할
    void Split() {
        float subWidth = bounds.Width / 2;
        float subHeight = bounds.Height / 2;
        float x = bounds.X;
        float y = bounds.Y;

        nodes[0] = new QuadtreeNode(RectF(x, y, subWidth, subHeight), level + 1); // 북서 (NW)
        nodes[1] = new QuadtreeNode(RectF(x + subWidth, y, subWidth, subHeight), level + 1); // 북동 (NE)
        nodes[2] = new QuadtreeNode(RectF(x, y + subHeight, subWidth, subHeight), level + 1); // 남서 (SW)
        nodes[3] = new QuadtreeNode(RectF(x + subWidth, y + subHeight, subWidth, subHeight), level + 1); // 남동 (SE)
    }

    // 객체가 속할 자식 노드의 인덱스를 결정
    int GetIndex(Collider* pObject) {
        RectF pRect = pObject->GetAABB(); // 객체의 AABB
        int index = -1;
        double verticalMidpoint = bounds.X + (bounds.Width / 2);
        double horizontalMidpoint = bounds.Y + (bounds.Height / 2);

        // 객체가 상단 사분면에 완전히 맞는지 확인
        bool topQuadrant = (pRect.Y < horizontalMidpoint && pRect.Y + pRect.Height < horizontalMidpoint);
        // 객체가 하단 사분면에 완전히 맞는지 확인
        bool bottomQuadrant = (pRect.Y > horizontalMidpoint);

        // 객체가 왼쪽 사분면에 완전히 맞는지 확인
        if (pRect.X < verticalMidpoint && pRect.X + pRect.Width < verticalMidpoint) {
            if (topQuadrant) {
                index = 0; // 북서
            } else if (bottomQuadrant) {
                index = 2; // 남서
            }
        }
        // 객체가 오른쪽 사분면에 완전히 맞는지 확인
        else if (pRect.X > verticalMidpoint) {
            if (topQuadrant) {
                index = 1; // 북동
            } else if (bottomQuadrant) {
                index = 3; // 남동
            }
        }
        return index;
    }

    // 객체를 쿼드트리에 삽입
    void Insert(Collider* pObject) {
        if (nodes[0] != nullptr) { // 분할되어 있으면
            int index = GetIndex(pObject);
            if (index != -1) { // 객체가 한 자식 노드에 완전히 속하면
                nodes[index]->Insert(pObject);
                return;
            }
        }

        // 분할되지 않았거나, 객체가 여러 자식 노드에 걸쳐 있으면 현재 노드에 추가
        objects.push_back(pObject);

        // 객체 수가 초과하고, 최대 레벨에 도달하지 않았으면 분할
        if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
            if (nodes[0] == nullptr) { // 아직 분할되지 않았으면
                Split();
            }

            // 현재 노드의 객체들을 자식 노드로 재분배
            int i = 0;
            while (i < objects.size()) {
                int index = GetIndex(objects[i]);
                if (index != -1) { // 한 자식 노드에 속하면 이동
                    nodes[index]->Insert(objects[i]);
                    objects.erase(objects.begin() + i); // 현재 노드에서는 제거
                } else {
                    i++; // 다음 객체로
                }
            }
        }
    }

    // 주어진 영역과 교차하는 모든 객체를 검색
    void Retrieve(std::vector<Collider*>& returnObjects, Collider* pObject) {
        RectF pRect = pObject->GetAABB();
        int index = GetIndex(pObject);

        if (nodes[0] != nullptr) { // 분할되어 있으면
            if (index != -1) { // 한 자식 노드에 완전히 속하면
                nodes[index]->Retrieve(returnObjects, pObject);
            } else { // 여러 자식 노드에 걸쳐 있으면 모든 자식 노드에서 검색
                for (int i = 0; i < 4; ++i) {
                    if (nodes[i]->bounds.IntersectsWith(pRect)) { // 교차하는 자식 노드만
                        nodes[i]->Retrieve(returnObjects, pObject);
                    }
                }
            }
        }
        // 현재 노드에 있는 객체들을 추가
        returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
    }

    // 쿼드트리 노드 그리기 (디버깅용)
    void Draw(Graphics* g, Pen* pen) {
        g->DrawRectangle(pen, bounds);
        for (int i = 0; i < 4; ++i) {
            if (nodes[i] != nullptr) {
                nodes[i]->Draw(g, pen);
            }
        }
    }
};

class CollisionManager : public SingletonT<CollisionManager> {
public:
	static bool ISBoxCollider(BoxCollider* p_box, POINT p_mousepos);

public:
    virtual ~CollisionManager();
    virtual void Initialize() override;
	virtual void CreateInitializeManager( ) override;
	

public:
    bool GetIsDebugDraw() const { return m_ISDebugDraw; }
    void SetIsDebugDraw(bool value) { m_ISDebugDraw = value; }
protected:
    bool m_ISDebugDraw = false;
    

protected:
    std::vector<Collider*> colliders;
    QuadtreeNode* quadtree; // 쿼드트리 루트 노드
    RectF worldBounds; // 전체 월드 영역


	// 이전 프레임의 충돌 쌍을 저장 (OnCollisionEnter/Exit 추적용)
    // std::set<std::pair<long, long>>은 정렬된 쌍을 저장하여 고유성 보장
    // 작은 ID를 먼저 오도록 정렬하여 (id1, id2)와 (id2, id1)을 동일하게 처리
    std::set<std::pair<long, long>> previousCollisions;
    std::set<std::pair<long, long>> currentFrameCollisions;




public:
	void ProcessDestroyCollider( );
	void RemoveCollider(Collider* p_collider);

protected:
	bool m_ISDirty = false;
	//std::vector<Collider*> m_PendingCollider;
	std::vector<Collider*> m_destroyQueue;
	virtual void MarkDirty( ) { m_ISDirty = true; }
	virtual void UpdateIfDirty( );


public:
	// 충돌체 추가
    void AddCollider(Collider* collider) 
	{ 
		colliders.push_back(collider); 
	}



    // 모든 충돌체에 대해 충돌 검사 수행
	void UpdateCollisions( );

	void DrawColliders(HDC p_hdc);

    // 모든 충돌체를 그림 (디버깅용)
	void DrawColliders(Graphics* g);


public:
	Component* Raycast(const PointF& rayStart, const PointF& rayEnd, PointF& outHitPoint);
	std::tuple<Component*, Gdiplus::PointF, bool> RaycastTuple(const PointF& rayStart, const PointF& rayEnd);

private:
	// 회전된 박스 Raycast
	bool RaycastBox(const BoxCollider& box, const PointF& rayStart, const PointF& rayEnd, PointF& outHitPoint);

	std::vector<PointF> GetRotatedCorners(const BoxCollider& box);

	// 원 Raycast
	bool RaycastCircle(const CircleCollider& circle, const PointF& rayStart, const PointF& rayEnd, PointF& outHitPoint);

	// 선분-선분 교차
	bool LineIntersect(const PointF& a1, const PointF& a2, const PointF& b1, const PointF& b2, PointF& intersection);
};