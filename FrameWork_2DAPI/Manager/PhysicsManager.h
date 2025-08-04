#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <memory>
#include <string>

#include "../Core/SingletonT.h"
#include "../Core/Vector.h"
#include "../Compoment/RigidbodyComponent.h"


//class RigidbodyComponent;

namespace Gdiplus{
    class Image;
};
using namespace Gdiplus;


enum class E_PHYSICSMATRIX
{
	DEFAULT,
	IGNORERAY,
	UI,

	MAX
};

class PhysicsManager : public SingletonT<PhysicsManager> {
protected:
	Vec2 Gravity = { 0, -9.81f };
	std::vector<std::unique_ptr<RigidbodyComponent>> m_PhysicsComVec;


public:
	const Vec2& GetGravity( ) { return Gravity; }


private:
	// 현재 사용하지 않음
	RigidbodyComponent* CreateRigidbodyCom( );

public:
	bool AddRigidbodyCom(RigidbodyComponent* p_com);
	void RemoveRigidbodyCom(RigidbodyComponent* p_com);
	void RemoveRigidbodyCom(long p_id);

public:
    virtual ~PhysicsManager();

	virtual void CreateInitializeManager( ) override;

	void UpdatePhysics( float p_dt );
	void DrawPhysics(HDC p_hdc);
};