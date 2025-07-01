#include "GameObject.h"
#include "../Compoment/Transform.h"

GameObject::GameObject()
{
	AddComponent<Transform>();
}

GameObject::~GameObject()
{
	RemoveComponent<Transform>();
}

void GameObject::Initialize()
{

}
