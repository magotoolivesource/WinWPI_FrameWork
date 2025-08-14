#include "Scene.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"





void Scene::UpdateLoop(float dt)
{
	
#pragma region 런타임 추가된 오브젝트및 컴포넌트 처리용

	ProcessNewObjects( );  // 새로운 오브젝트를 프레임 시작 시 반영
	ProcessRunTimeAddComponentObject( );  // 런타임시 오브젝트에 addcomponent 된것들 호출 적용

#pragma endregion


	UpdateIfDirty( );

	//for ( auto& obj : m_AllOjects )
	//	obj->Update(dt);
	// 
	//for ( auto& obj : m_sortedObjects )
	//	obj->Update(dt);

	for ( size_t i = 0; i < m_sortedObjects.size(); i++ )
	{
		m_sortedObjects[ i ]->Update(dt);
	}
}

void Scene::Render(HDC p_hdc)
{
	//for ( auto& obj : m_AllOjects )
	//	obj->Render(p_hdc);

	for ( auto& obj : m_sortedObjects )
		obj->Render(p_hdc);


	//ProcessDestroyQueue( ); // Render 이후 삭제 처리
}

void Scene::AddRuntimeDirtyAddComponent(GameObject* obj)
{
	m_RuntimeAddComponentObjects.push_back(obj);

	//MarkRuntimeAddComponentDirty( );
}

void Scene::ProcessNewObjects( )
{

	int size = m_pendingStartObjects.size( );
	if ( size == 0 && m_pendingObjects.empty( ) )
		return; // 새로운 오브젝트가 없으면 리턴

	isDirty = true;

	// Start 호출
	for ( auto* obj : m_pendingStartObjects ) {
		obj->Start( );
		m_sortedObjects.push_back(obj);
	}
	m_pendingStartObjects.clear( );

	// m_pendingObjects를 m_AllOjects로 이동
	for ( auto& obj : m_pendingObjects ) {
		m_AllOjects.emplace_back(std::move(obj));
	}
	m_pendingObjects.clear( );

}

void Scene::ProcessRunTimeAddComponentObject( )
{
	if ( m_RuntimeAddComponentObjects.empty( ) )
		return;

	for ( auto* obj : m_RuntimeAddComponentObjects ) {
		obj->Start( );
	}

	m_RuntimeAddComponentObjects.clear( );
}

void Scene::Prev_DestroyQueueObjects_AllRemoveComponent( )
{
	// 전체적으로 오브젝트안에 컴포넌트들 지우도록 처리 
	for ( auto* obj : m_destroyQueue )
	{
		obj->DestroyGameObject( );
	}
}

void Scene::ProcessDestroyQueue( )
{


	for ( auto* obj : m_destroyQueue ) {

		//obj->AllDirectDestroyComponent( );

		// m_sortedObjects에서 제거
		m_sortedObjects.erase(
			std::remove(m_sortedObjects.begin( ), m_sortedObjects.end( ), obj),
			m_sortedObjects.end( )
		);

		// m_AllOjects에서 제거 + unique_ptr 메모리 해제
		auto it = std::find_if(m_AllOjects.begin( ), m_AllOjects.end( ),
			[obj] (const std::unique_ptr<GameObject>& p) { return p.get( ) == obj; });
		if ( it != m_AllOjects.end( ) ) {
			m_AllOjects.erase(it);
		}
	}
	m_destroyQueue.clear( );
}


GameObject* Scene::CreateObject(const std::string& name)
{
	auto obj = std::make_unique<GameObject>( this );
	obj->SetName(name);
	obj->InitCreateScene(this);

	GameObject* ptr = obj.get( );

	//m_AllOjects.emplace_back(std::move(obj));
	//m_sortedObjects.emplace_back(ptr);

	// 일단 pending 리스트에 보관
	m_pendingStartObjects.push_back(ptr);
	m_pendingObjects.emplace_back(std::move(obj));

	isDirty = true; // 새로운 오브젝트가 추가되었으므로 isDirty를 true로 설정

	return ptr;
}

void Scene::DestroyObject(GameObject* obj) {

	//// for(auto item : objects )
	//for ( size_t i = 0; i < m_AllOjects.size( ); i++ ) {
	//	if ( m_AllOjects[ i ].get( ) == obj ) {

	//		m_sortedObjects.erase(m_sortedObjects.begin( ) + i);
	//		m_AllOjects[ i ].release( ); // Release the unique_ptr
	//		m_AllOjects.erase(m_AllOjects.begin( ) + i);
	//		return;
	//	}
	//}



	// 자식 오브젝트 재귀 삭제
	auto& children = obj->transform->GetChildren( );
	for ( auto* childTransform : children ) {
		if ( childTransform && childTransform->owner ) {
			DestroyObject(childTransform->owner);
		}
	}


	// 중복 삭제 방지
	if ( std::find(m_destroyQueue.begin( ), m_destroyQueue.end( ), obj) == m_destroyQueue.end( ) ) {
		m_destroyQueue.push_back(obj);

		//obj->DestroyGameObject( );
	}


	MarkDirty( );
}

void Scene::UpdateIfDirty( )
{
	//if ( !isDirty ) return;

	////std::vector<GameObject*> sortedObjects(m_AllOjects.begin( ), m_AllOjects.end( ));
	//std::sort(m_sortedObjects.begin( ), m_sortedObjects.end( ), [ ] (GameObject* a, GameObject* b) {
	//	return a->transform->GetWorldDepth( ) < b->transform->GetWorldDepth( );
	//	});

	//isDirty = false;



	if ( isDirty )
	{
		std::sort(m_sortedObjects.begin( ), m_sortedObjects.end( ), [ ] (GameObject* a, GameObject* b) {
			return a->transform->GetWorldDepth( ) < b->transform->GetWorldDepth( );
			});
		isDirty = false;
	}

	
	


}

//void Scene::UpdateIfRuntimeAddComponentDirty( )
//{
//
//}

