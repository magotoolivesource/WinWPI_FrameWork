#pragma once
#include <Compoment/Component.h>
#include <Core/Vector.h>
#include <vector>
#include <typeinfo>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

//#include "ZombieFactoryManager.h"


class UI_SelectBTNCom;
class Button;
class NormalZombie;

// 팩토리패턴들 적용
// https://chatgpt.com/c/68a654aa-5f48-8323-8325-6b9dd258d170

class ZombiStateData
{
public:
	ZombiStateData( ) = default;
	ZombiStateData( int hp, int speed)
		: HP(hp), Speed(speed) {}

	virtual ~ZombiStateData( ) = default;
	

	int HP = 0;
	int Speed = 0;
};



//class Base_ZombiActor;
class Base_ZombiActor : public Component
{
public:
	Base_ZombiActor* Create_ZombiActorObject( );

public:
	Base_ZombiActor();
	virtual ~Base_ZombiActor( ) override;

public:
	void SetZombiStateData(int hp, int speed);
	ZombiStateData& GetZombiStateData( );

public:
	virtual void InitSettings( );
	void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;

protected:
	//ZombiStateData* m_ZombiStateData = nullptr;
	ZombiStateData m_ZombiStateData;
};

//
//// =============================
//// 자동 등록 헬퍼
//// =============================
//template <typename T>
//class ZombieAutoRegister {
//public:
//	ZombieAutoRegister(const std::string& typeName) {
//		ZombieFactoryManager::Instance( ).RegisterZombie(
//			typeName,
//			[ ] ( ) { return std::make_unique<T>( ); }
//		);
//	}
//};
//
//
//template <typename Derived>
//class AutoZombie : public Base_ZombiActor {
//public:
//	AutoZombie( )
//	{
//
//	}
//	//AutoZombie(const std::string& name, int hp, int speed)
//	//	//: ZombieComponent(name, hp, speed) 
//	//{
//	//	
//	//}
//	virtual ~AutoZombie( ) override = default;
//
//protected:
//	//inline static ZombieAutoRegister<Derived> reg{ Derived::TypeName( ) };
//
//	inline static ZombieAutoRegister<Derived> reg
//	{ 
//		//std::string( typeid( Derived ).name( ) ) // typeid( Derived )를 통해 타입 정보를 가져옴
//		std::string( typeid( Derived ).name( ) )
//		
//		//Derived::TypeName( )
//	};
//
//
//};
//
//
//


//
//// =============================
//// CRTP 기반 AutoZombie
//// =============================
//template <typename Derived>
//class AutoZombie : public Base_ZombiActor {
//public:
//	//AutoZombie(const std::string& name, int hp, int speed)
//	//	: ZombieComponent(name, hp, speed) {
//	//}
//
//	AutoZombie(){
//	}
//
//	virtual ~AutoZombie( ) override = default;
//
//protected:
//	struct Registrar {
//		Registrar( ) {
//			ZombieFactoryManager::GetI( )->RegisterZombie<Derived>( );
//		}
//	};
//	static inline Registrar registrar{}; // 정적 등록
//};


