#include "Component.h"
#include "../Core/GameObject.h"
#include "Transform.h"

void Component::Initialize_AddCompoment() 
{ 
	transform = owner->GetComponent<Transform>();

}

void Component::RemoveCompoment( )
{

}
