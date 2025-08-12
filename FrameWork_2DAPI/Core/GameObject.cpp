#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Compoment/Transform.h"
#include "MyString.h"
#include <algorithm>



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
	m_ISObjectDirty = E_GameObjectDirtyType::InitCreateObject;

    Initialize();

}

GameObject::~GameObject()
{
	Release( );
	RemoveComponent<Transform>();
}

void GameObject::Release( )
{

	for ( auto& it : components )
	{
		it.second.release( );
	}

	components.clear( );

	//auto it = components.find(std::type_index(typeid( T )));
	//if ( it != components.end( ) )
	//{
	//	T* com = dynamic_cast< T* >( it->second.get( ) );
	//	com->RemoveCompoment( );
	//	//delete it->second.get( );
	//	it->second.release( );
	//	components.erase(it);

	//	return true;
	//}


}

void GameObject::DestroyGameObject( )
{

}

void GameObject::SetName(const std::string& n)
{
	name = n;
	wname = MyString::StringToWString(name);
}

void GameObject::Initialize()
{
    InitCreateTransform();
}

void GameObject::InitCreateTransform()
{
    Transform* comp = new Transform();
    //comp->owner = this;
    //comp->Initialize_AddCompoment(); // 컴포넌트 초기화 호출 추가
    //components[std::type_index(typeid(Transform))].reset(comp);

	RegistComponent(comp);

    transform = comp;

	
}

void GameObject::InitAdjustCompnent( )
{
	//std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
	////std::unordered_map<std::type_index, std::unique_ptr<Component>> loopcomponents;
	//std::vector<Component*> m_loopcomponents;

	if ( m_ISObjectDirty != E_GameObjectDirtyType::None )
	{
		m_loopcomponents.clear( );
		std::transform(components.begin( ), components.end( ),
			std::back_inserter(m_loopcomponents),
			[ ] (const auto& pair) {
				return pair.second.get( );
			}
		);
	}

	m_ISObjectDirty = E_GameObjectDirtyType::None;

	//if ( GetISAddComponent( ) )
	//{

	//}


}

void GameObject::SetISObjectDirty(E_GameObjectDirtyType p_type)
{
	m_ISObjectDirty = ( E_GameObjectDirtyType )(( int )m_ISObjectDirty | (int)p_type);
}

bool GameObject::GetISCreateObject( )
{
	return ( bool ) ( (int)m_ISObjectDirty | (int)E_GameObjectDirtyType::InitCreateObject );
}

bool GameObject::GetISAddComponent( )
{
	return ( bool ) ( ( int ) m_ISObjectDirty | ( int ) E_GameObjectDirtyType::AddComponent );
}
