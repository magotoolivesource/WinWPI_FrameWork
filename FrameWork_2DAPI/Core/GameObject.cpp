#include "GameObject.h"
#include "../Compoment/Transform.h"

GameObject::GameObject()
{
	//AddComponent<Transform>();
    Initialize();

}

GameObject::~GameObject()
{
	RemoveComponent<Transform>();
}

void GameObject::Initialize()
{
    InitCreateTransform();
}

void GameObject::InitCreateTransform()
{
    Transform* comp = new Transform();
    comp->owner = this;
    comp->Initialize_AddCompoment(); // 컴포넌트 초기화 호출 추가
    components[std::type_index(typeid(Transform))].reset(comp);

    transform = comp;
}
