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

GameObject::GameObject() : m_LinkScene(nullptr)
{
    Initialize();
}

GameObject::GameObject(Scene* p_linkscene) : m_LinkScene(p_linkscene)
{
	Initialize( );
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
	//AllDirectDestroyComponent( );
	
	DirectAllDestroyComponents( );
}
//
//void GameObject::AllDirectDestroyComponent( )
//{
//	if ( m_pendingStartComponents.empty( ) )
//	{
//		for ( auto item : m_pendingStartComponents )
//		{
//			// 중복 삭제 방지
//			if ( std::find(m_destroyQueue.begin( ), m_destroyQueue.end( ), item) == m_destroyQueue.end( ) ) {
//				m_destroyQueue.push_back(item);
//			}
//		}
//	}
//
//	if ( !m_activeComponents.empty( ) )
//	{
//		for ( auto item : m_activeComponents )
//		{
//			// 중복 삭제 방지
//			if ( std::find(m_destroyQueue.begin( ), m_destroyQueue.end( ), item) == m_destroyQueue.end( ) ) {
//				m_destroyQueue.push_back(item);
//			}
//		}
//	}
//
//}

void GameObject::SetName(const std::string& n)
{
	name = n;
	wname = MyString::StringToWString(name);
}

void GameObject::Initialize()
{
	m_ISObjectDirty = E_GameObjectDirtyType::InitCreateObject;

    InitCreateTransform();
}

void GameObject::InitCreateTransform()
{
    Transform* comp = new Transform();
    //comp->owner = this;
    //comp->Initialize_AddCompoment(); // 컴포넌트 초기화 호출 추가
    //components[std::type_index(typeid(Transform))].reset(comp);

	RegistComponent(comp, true);

    transform = comp;

	
}

void GameObject::InitAdjustCompnent( )
{
	//std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
	////std::unordered_map<std::type_index, std::unique_ptr<Component>> loopcomponents;
	//std::vector<Component*> m_loopcomponents;

	if ( m_ISObjectDirty != E_GameObjectDirtyType::None )
	{

		//m_loopcomponents.clear( );
		//std::transform(components.begin( ), components.end( ),
		//	std::back_inserter(m_loopcomponents),
		//	[ ] (const auto& pair) {
		//		return pair.second.get( );
		//	}
		//);

	}

	m_ISObjectDirty = E_GameObjectDirtyType::None;

	//if ( GetISAddComponent( ) )
	//{

	//}


}

void GameObject::SetISObjectDirty(E_GameObjectDirtyType p_type)
{
	m_ISObjectDirty = ( E_GameObjectDirtyType )(( int )m_ISObjectDirty | (int)p_type);


	if ( p_type == E_GameObjectDirtyType::AddComponent )
	{
		if( m_LinkScene )
			m_LinkScene->AddRuntimeDirtyAddComponent(this);
	}

}

bool GameObject::GetISCreateObject( )
{
	return ( bool ) ( (int)m_ISObjectDirty | (int)E_GameObjectDirtyType::InitCreateObject );
}

bool GameObject::GetISAddComponent( )
{
	return ( bool ) ( ( int ) m_ISObjectDirty | ( int ) E_GameObjectDirtyType::AddComponent );
}

void GameObject::ProcessNewComponents( )
{
	if ( m_pendingStartComponents.empty( ) )
		return;

	m_tempstartcomponents.clear( );
	for ( auto* comp : m_pendingStartComponents ) {
		//comp->Start( );
		m_activeComponents.push_back(comp);
		components[ std::type_index(typeid( *comp )) ] = std::move(FindPendingUniquePtr(comp));

		m_tempstartcomponents.push_back(comp);
	}
	m_pendingStartComponents.clear( );
	m_pendingComponents.clear( );


	// Start 지연 호출적용하기
	for(auto* comp : m_tempstartcomponents ) {
		comp->Start( );
	}
	m_tempstartcomponents.clear( );

}

void GameObject::ProcessDestroyComponents( )
{

	for ( auto* comp : m_destroyQueue ) {

		comp->RemoveCompoment( );

		// 활성 리스트에서 제외
		m_activeComponents.erase(
			std::remove(m_activeComponents.begin( ), m_activeComponents.end( ), comp),
			m_activeComponents.end( )
		);

		components.erase(std::type_index(typeid( *comp )));
	}
	m_destroyQueue.clear( );

}

void GameObject::DirectAllDestroyComponents( )
{
	// 강제로 남은 컴포넌트들 다지우기
	for ( auto item : m_activeComponents )
	{
		item->RemoveCompoment( );
	}
	for ( auto item : m_pendingStartComponents )
	{
		item->RemoveCompoment( );
	}

	m_activeComponents.clear( );
	m_pendingStartComponents.clear( );
	
	m_pendingComponents.clear( );
	m_destroyQueue.clear( );
	components.clear( );
}

std::unique_ptr<Component> GameObject::FindPendingUniquePtr(Component* comp) {

	auto it = std::find_if(m_pendingComponents.begin( ), m_pendingComponents.end( ),
		[comp] (const std::unique_ptr<Component>& p) { return p.get( ) == comp; });
	if ( it != m_pendingComponents.end( ) ) {
		auto ptr = std::move(*it);
		m_pendingComponents.erase(it);
		return ptr;
	}
	return nullptr;
}

//void GameObject::InitializeComponents( )
//{
//	for ( auto& comp : m_newlyAddedComponents ) {
//		//comp->Initialize_AddCompoment( );
//		comp->Start( ); // Now we call Start here
//	}
//	m_newlyAddedComponents.clear( );
//}

void GameObject::Start( )
{
	//////if ( m_ISSceneAddInit ) return;

	//////if ( !active ) return;

	//////m_ISSceneAddInit = true;

	//////for (auto& [_, comp] : components) comp->Start();
	////for ( auto& comp : components )
	////{
	////	comp.second->Start( );
	////}




	////if ( !GetISCreateObject() ) return;

	////// Call Start on all components added at creation
	////for ( auto& [_, comp] : components ) {
	////	comp->Start( );
	////}
	//////m_isInitialized = true;
	////m_ISObjectDirty = E_GameObjectDirtyType::None;


	//ProcessNewComponents( );
	//m_ISObjectDirty = E_GameObjectDirtyType::None;

	InitAddComponentStart( );
}

void GameObject::InitAddComponentStart( )
{
	ProcessNewComponents( );
	m_ISObjectDirty = E_GameObjectDirtyType::None;
}

void GameObject::Update(float dt) {
	//if ( !m_ISSceneAddInit ) return;

	if ( !active ) return;



	////for (auto& [_, comp] : components) comp->Update(dt);
	//for ( auto& comp : components )
	//{
	//	if ( comp.second->IsEnabled( ) ) 
	//	{
	//		comp.second->Update(dt);
	//	}
	//}


	for ( auto* comp : m_activeComponents ) {
		comp->Update(dt);
	}
}

void GameObject::Render(HDC hdc) {
	//if ( !m_ISSceneAddInit ) return;

	if ( !active ) return;

	////for (auto& [_, comp] : components) comp->Render(hdc);
	//for ( auto& comp : components )
	//{
	//	comp.second->Render(hdc);
	//}

	for ( auto* comp : m_activeComponents ) {
		comp->Render(hdc);
	}


	// 매니저 이후에 소멸 처리들 호출 되어야지 되기때문에 바꿈
	ProcessDestroyComponents( );
}