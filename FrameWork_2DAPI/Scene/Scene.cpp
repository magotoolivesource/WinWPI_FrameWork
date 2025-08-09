#include "Scene.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"



void Scene::UpdateIfDirty( )
{
	if ( !isDirty ) return;


	//std::vector<GameObject*> sortedObjects(m_AllOjects.begin( ), m_AllOjects.end( ));
	std::sort(m_sortedObjects.begin( ), m_sortedObjects.end( ), [ ] (GameObject* a, GameObject* b) {
		return a->transform->GetWorldDepth( ) < b->transform->GetWorldDepth( );
		});


	isDirty = false;
}

void Scene::Update(float dt)
{
	UpdateIfDirty( );

	for ( auto& obj : m_AllOjects )
		obj->Update(dt);
}

void Scene::Render(HDC p_hdc)
{
	for ( auto& obj : m_AllOjects )
		obj->Render(p_hdc);
}
