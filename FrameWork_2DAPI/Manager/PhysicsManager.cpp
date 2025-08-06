#include "PhysicsManager.h"
#include <windows.h>
#include <gdiplus.h>

#include "../Compoment/RigidbodyComponent.h"

//#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib")



bool PhysicsManager::AddRigidbodyCom(RigidbodyComponent* p_com)
{
	m_PhysicsComVec.emplace_back(std::move(p_com));
	return false;
}

RigidbodyComponent* PhysicsManager::CreateRigidbodyCom( )
{
	//RigidbodyComponent

	auto rigidbody = std::make_unique<RigidbodyComponent>( );
	m_PhysicsComVec.emplace_back(std::move(rigidbody));
	//RigidbodyComponent* outcom = rigidbody.get( );
	return rigidbody.get( );
}

void PhysicsManager::RemoveRigidbodyCom(RigidbodyComponent* p_com)
{
}

void PhysicsManager::RemoveRigidbodyCom(long p_id)
{
}

PhysicsManager::~PhysicsManager()
{
	for ( size_t i = 0; i < m_PhysicsComVec.size(); i++ )
	{
		m_PhysicsComVec[ i ].release( );
	}
}

void PhysicsManager::CreateInitializeManager( )
{
	__super::CreateInitializeManager( );


}

void PhysicsManager::UpdatePhysics(float p_dt)
{
	for ( size_t i = 0; i < m_PhysicsComVec.size( ); i++ )
	{
		m_PhysicsComVec[ i ].get( )->PhysicsUpdate(p_dt);
	}
}

void PhysicsManager::DrawPhysics(HDC p_hdc)
{

}
