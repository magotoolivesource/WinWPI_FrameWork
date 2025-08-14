#include "Component.h"
#include "../Core/GameObject.h"
#include "Transform.h"

void Component::Initialize_AddCompoment() 
{ 
	transform = owner->GetComponent<Transform>();
	owner->SetISObjectDirty(E_GameObjectDirtyType::AddComponent);

}

void Component::RemoveCompoment( )
{
	m_ISDestroyIng = true;
	enabled = false;
	owner->SetISObjectDirty(E_GameObjectDirtyType::RemoveComponent);

}
