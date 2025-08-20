#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

#include <type_traits>
#include <iostream>

#include "../Compoment/Component.h"
//#include "../Compoment/Transform.h"

//class Component;
class Transform;
class Scene;


enum class E_GameObjectDirtyType
{
	None = 0,
	InitCreateObject = 1 << 0,
	AddComponent = 1 << 1,
	RemoveComponent = 1 << 2,

	AllType = InitCreateObject | AddComponent | RemoveComponent,
};


// 1번 https://chatgpt.com/c/685d15a6-5204-8013-ae97-2bd6dfe11517
// 2번 https://chatgpt.com/c/689005d8-abc4-8013-b786-dcc76a8e9d17
// // 3번 https://gemini.google.com/app/36a5f1ab8451eba4
// 3번 https://chatgpt.com/c/689c15b8-1768-8329-9950-f6f603b91a95
// 3번 https://chatgpt.com/c/689c1c49-4af0-832d-a0c3-f1afc0ea8551

class GameObject
{
public:
    //std::string name;
    ////std::vector<std::unique_ptr<Component>> components;
    //std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

    //template<typename T, typename... Args>
    //T* AddComponent(Args&&... args) {
    //    T* comp = new T(std::forward<Args>(args)...);
    //    comp->gameObject = this;
    //    components.emplace_back(comp);
    //    return comp;
    //}

    ////void Start() {
    ////    for (auto& c : components) c->Start();
    ////}

    ////void Update() {
    ////    for (auto& c : components) c->Update();
    ////}

    //template <typename T>
    //T* GetComponent() {
    //    auto it = components.find(std::type_index(typeid(T)));
    //    return (it != components.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
    //}

    //void Start() {
    //    for (auto& [_, comp] : components) comp->Start();
    //}
    //void Update(float dt) {
    //    for (auto& [_, comp] : components) comp->Update(dt);
    //}
    //void Render(HDC hdc) {
    //    for (auto& [_, comp] : components) comp->Render(hdc);
    //}


protected:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

	////std::unordered_map<std::type_index, std::unique_ptr<Component>> loopcomponents;
	//std::vector<Component*> m_loopcomponents;
	//std::vector<Component*> m_initcomponents;

    // 새로 추가
    std::unordered_set<std::string> tags;
    bool active = true;
    std::string name;
	std::wstring wname;

private:
	Scene* m_LinkScene = nullptr;
	void InitCreateScene(Scene* p_linkscene);

public:
	void SetSceneMakeDirty( );
	friend Scene;


public:
    GameObject();
	GameObject(Scene* p_linkscene);
    virtual ~GameObject();


	void SetName(const std::string& n);
    const std::string& GetName() const { return name; }
    const std::wstring& GetNameW() const { return wname; }

    void SetActive(bool a) { active = a; }
    bool IsActive() const { return active; }

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args);

    template <typename T, typename... Args>
	bool RemoveComponent( );

private:
	template <typename T>
	void RegistComponent(T* p_com, bool p_isdirect = false);


public:
    template <typename T>
    T* GetComponent() {
        auto it = components.find(std::type_index(typeid(T)));

		T* outcom = ( it != components.end( ) ) ? dynamic_cast< T* >( it->second.get( ) ) : nullptr;
		if ( outcom == nullptr )
			outcom = GetComponentDynamic<T>( );
		return outcom;
    }
    

	void Release( );
	void DestroyGameObject( );

private:
	//void AllDirectDestroyComponent( );

protected:
	template <typename T>
	T* GetComponentDynamic( ) {
		for ( const auto& comp : components )
		{
			if ( T* casted = dynamic_cast< T* >( comp.second.get( ) ) )
			{
				return casted;
			}
		}

		return nullptr;
	}


public:
	void Start( );
	void InitAddComponentStart( );

	void Update(float dt);
	void Render(HDC hdc);


    void AddTag(const std::string& tag) {
        tags.insert(tag);
    }

    bool HasTag(const std::string& tag) const {
        return tags.find(tag) != tags.end();
    }
	std::string GetTag() {
		if ( tags.empty( ) || tags.size( ) <= 0 )
			return "DefaultTag";
		else
			return *tags.begin( );
	}


protected:
    void Initialize();
	void InitCreateTransform();

public:
    Transform* transform;// = nullptr;


protected:
	//bool m_ISSceneAddInit = false;

	//bool isDirty = true;
	E_GameObjectDirtyType m_ISObjectDirty = E_GameObjectDirtyType::None;

protected:
	void InitAdjustCompnent( );

public:
	void SetISObjectDirty(E_GameObjectDirtyType p_type);
	E_GameObjectDirtyType GetISDirty( ) { return m_ISObjectDirty; }
	bool GetISCreateObject( );
	bool GetISAddComponent( );


private:


//
//protected:
//	std::vector<Component*> m_newlyAddedComponents;
//
//public:
//	void InitializeComponents( ); // New method to handle deferred initialization

protected:
	std::vector<Component*> m_activeComponents;
	// 추가 대기
	std::vector<std::unique_ptr<Component>> m_pendingComponents;
	std::vector<Component*> m_pendingStartComponents;

	// 삭제 예약
	std::vector<Component*> m_destroyQueue;

protected:
	void ProcessNewComponents( );
	void ProcessDestroyComponents( );

protected:
	void DirectAllDestroyComponents( );

private:
	// pendingComponents에서 해당 포인터의 unique_ptr을 찾아 반환
	std::unique_ptr<Component> FindPendingUniquePtr(Component* comp);

};

template<typename T, typename ...Args>
inline T* GameObject::AddComponent(Args && ...args)
{

    //if (typeid(T) == typeid(Transform))
    //{
    //    _ASSERT(false && "Transform 만들수 없습니다.");
    //    return nullptr; // Transform은 GameObject에 자동으로 추가되므로, 명시적으로 추가하지 않음
    //}
    if constexpr (std::is_same<T, Transform>::value) {
        std::cerr << "[경고] Transform은 자동 생성되므로 추가하지 마세요.";
        return GetComponent<T>();
    }

 //   T* comp = new T(std::forward<Args>(args)...);


 //   //comp->owner = this;
 //   //comp->Initialize_AddCompoment(); // 컴포넌트 초기화 호출 추가
 ////   components[std::type_index(typeid(T))].reset(comp);
	////m_initcomponents.push_back(comp);



	//RegistComponent(comp);
 //   return comp;



	T* comp = new T(std::forward<Args>(args)...);

	RegistComponent(comp);

	return comp;

}

template<typename T, typename ...Args>
inline bool GameObject::RemoveComponent( ) {
//	//T* comp = new T(std::forward<Args>(args)...);
//	//comp->owner = this;
//	//auto it = components.find(std::type_index(typeid(T)));
//
//
//
//	auto it = components.find(std::type_index(typeid( T )));
//	if ( it != components.end( ) )
//	{
//		T* com = dynamic_cast< T* >( it->second.get( ) );
//		com->RemoveCompoment( );
//		//delete it->second.get( );
//		it->second.release( );
//		components.erase(it);
//
//		return true;
//	}
//
//
//	//if ( components[ std::type_index(typeid( T )) ] )
//	//{
//	//	auto com = components[ std::type_index(typeid( T )) ];
//
//	//	com.get()->RemoveCompoment();
//	//	delete com.get( );
//	//	com.release( );
//	//	components.erase(std::type_index(typeid( T )));
//
//	//	return true;
//	//}
//
//
////     if (components[std::type_index(typeid(T))])
////     {
//		 //RemoveCompoment
//
////         delete components[std::type_index(typeid(T))].get();
////         components[std::type_index(typeid(T))].release();// = nullptr;
//
////         components.erase(std::type_index(typeid(T)));
//
////         return true;
////     }
//
//	return false;


	auto it = components.find(std::type_index(typeid( T )));
	if ( it != components.end( ) ) {
		Component* comp = it->second.get( );
		comp->RemoveCompoment( );

		// 중복 삭제 방지
		if ( std::find(m_destroyQueue.begin( ), m_destroyQueue.end( ), comp) == m_destroyQueue.end( ) ) {
			m_destroyQueue.push_back(comp);
		}
		//// 삭제 예약
		//m_destroyQueue.push_back(comp);

		//// 활성 리스트에서 제외
		//m_activeComponents.erase(
		//	std::remove(m_activeComponents.begin( ), m_activeComponents.end( ), comp),
		//	m_activeComponents.end( )
		//);

		SetISObjectDirty(E_GameObjectDirtyType::RemoveComponent);
		return true;
	}
	return false;
}

template<typename T>
inline void GameObject::RegistComponent(T* p_com, bool p_isdirect)
{
	//p_com->owner = this;
	//p_com->Initialize_AddCompoment( ); // 컴포넌트 초기화 호출 추가
	////components[ std::type_index(typeid( T )) ].reset(p_com); // 기존 중복 지우기

	//T* ptr = p_com.get( );
	//m_pendingStartComponents.push_back(ptr);
	//m_pendingComponents.emplace_back(std::move(p_com));



	p_com->owner = this;
	p_com->Initialize_AddCompoment( );

	if ( p_isdirect )
	{
		components[std::type_index(typeid(T))].reset(p_com);
	}
	else
	{
		//T* ptr = comp->get( );
		m_pendingStartComponents.push_back(p_com);
		m_pendingComponents.emplace_back(std::move(p_com));

		SetISObjectDirty(E_GameObjectDirtyType::AddComponent);
	}
	

}
