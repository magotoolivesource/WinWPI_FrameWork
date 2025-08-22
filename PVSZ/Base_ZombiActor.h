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

#include "PVSZ_ALLTableDatas.h"


class UI_SelectBTNCom;
class Button;
class NormalZombie;
class Collider;
class ImageComponent;
class ImageAni_Component;
class Move_Com;
class UtilTimer;

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
	ZombiATKData m_ZombiATKData = {1.f};

protected:
	virtual void Collider_TriggerEnter(Collider* src_other, Collider* dest_other, void* p_data) {};
	virtual void Collider_TriggerStay(Collider* src_other, Collider* dest_other, void* p_data) {};
	virtual void Collider_TriggerExit(Collider* src_other, Collider* dest_other, void* p_data) {};

protected:
	GameObject* m_TargetPlant = nullptr;

public:
	virtual void SetDieEvent( );
	virtual void DestroyEvent( );
	virtual void DestroyEvent_CallFN(UtilTimer* p_timer, void* p_data);
	virtual void SetDamage(float p_atk);
	virtual void UpdateUI( );

	virtual void ResetAllDatas( );
	virtual bool ISDie( );

	virtual void SetMove(bool p_ismove);
protected:
	bool m_ISDie = false;
	ImageComponent* m_ImageCom = nullptr;
	ImageAni_Component* m_LinkImaeAniCom = nullptr;
	Move_Com* m_MoveCom = nullptr;
	float m_MoveSpeed = 0.f; // 이동 속도
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


