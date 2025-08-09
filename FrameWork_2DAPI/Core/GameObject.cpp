#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Compoment/Transform.h"

void GameObject::InitCreateScene(Scene* p_linkscene)
{
	m_LinkScene = p_linkscene;
}

void GameObject::SetSceneMakeDirty()
{
	if (m_LinkScene)
		m_LinkScene->MarkDirty();
}

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
