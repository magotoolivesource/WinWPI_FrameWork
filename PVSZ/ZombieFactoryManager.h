#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <format>

#include <Core/SingletonT.h>
#include <Core/GameObject.h>


#include "InGame_PVSZ.h"
#include "Base_ZombiActor.h"
//#include "NormalZombie.h"



//
//class UI_SelectBTNCom;
//class Button;
//class Base_ZombiActor;
//
//class ZombieFactoryManager : public SingletonT< ZombieFactoryManager>
//{
//public:
//	ZombieFactoryManager();
//	virtual ~ZombieFactoryManager( ) override;
//
//
//public:
//	using Creator = std::function<std::unique_ptr<Base_ZombiActor>( )>;
//	void RegisterZombie(const std::string& typeName, Creator creator);
//
//	// deepcopy 생성자 적용
//	std::unique_ptr<Base_ZombiActor> CreateZombie(const std::string& typeName);
//
//private:
//	std::unordered_map<std::string, Creator> creators;
//
//};




class Base_ZombiActor; // 전방 선언
class NormalZombie; // 전방 선언




class ZombieFactoryManager : public SingletonT< ZombieFactoryManager >
{
public:
	//using Creator = std::function<std::unique_ptr<Base_ZombiActor>( )>;
	using Creator = std::function<Base_ZombiActor*( )>;

	static unsigned int m_UID;
public:
	ZombieFactoryManager() = default;
	virtual ~ZombieFactoryManager( ) override = default;

	template <typename T>
	void RegisterZombie( ) {
		std::string key = typeid( T ).name( ); // RTTI 문자열을 key로 사용
		//creators[ key ] = [ ] ( ) { return std::make_unique<T>( ); };

		//creators[ key ] = [] ( ) 
		//{
		//	return nullptr;
		//};

		creators[ key ] = [] ( ) -> Base_ZombiActor*
			{
				string keyid = format("ZombiObj_{}", ZombieFactoryManager::m_UID++);
				GameObject* zombiobj = InGame_PVSZ::AddGameObject(keyid);
				T* resultt = zombiobj->AddComponent<T>( ); // T 타입의 컴포넌트 추가
				return resultt;// std::make_unique<T>( );
			};
	}

	//template <typename T>
	//std::unique_ptr<T> CreateZombie( ) {
	//	std::string key = typeid( T ).name( );
	//	auto it = creators.find(key);
	//	if ( it != creators.end( ) ) {
	//		return std::unique_ptr<T>(static_cast< T* >( it->second( ).release( ) ));
	//	}
	//	return nullptr;
	//}

	template <typename T>
	T* CreateZombie( ) {
		std::string key = typeid( T ).name( );
		auto it = creators.find(key);
		if ( it != creators.end( ) ) {
			return dynamic_cast< T* >( it->second( ) );
		}
		return nullptr;
	}

protected:
	std::unordered_map<std::string, Creator> creators;
};



// =============================
// CRTP 기반 AutoZombie
// =============================
template <typename TDerived>
class AutoZombie : public Base_ZombiActor {
public:
	//AutoZombie(const std::string& name, int hp, int speed)
	//	: ZombieComponent(name, hp, speed) {
	//}

	AutoZombie( ) {
	}

	virtual ~AutoZombie( ) override = default;

protected:
	struct Registrar {
		Registrar( ) {
			ZombieFactoryManager::GetI( )->RegisterZombie<TDerived>( );
		}
	};
	static inline Registrar registrar; // 정적 등록
};




