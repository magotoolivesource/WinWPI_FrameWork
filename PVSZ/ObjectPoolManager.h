#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
#include <memory>
#include <typeindex>
#include <Core/SingletonT.h>


class GameObject;
class Component;

class Pea_Com;
class Sunflower_Com;
class Sun_Com;
class Plant_PeaShooter;

class ObjectPoolManager : public SingletonT< ObjectPoolManager>
{
public:
	ObjectPoolManager();
	virtual ~ObjectPoolManager( ) override;

public:
	void InitSettings( );
	virtual void CreateInitializeManager( ) override;


//protected:
//	std::vector<GameObject*> m_AllGameObjectVec;
//	std::set< GameObject*> m_AllGameObjectSet;
//
private:
	std::unordered_map<std::type_index, std::queue<std::shared_ptr<GameObject>>> pool;

public:
	template <typename T, typename... Args>
	std::shared_ptr<T> GetObject(Args&&... args) {
		auto& q = pool[ std::type_index(typeid( T )) ];

		if ( !q.empty( ) ) {
			auto obj = std::static_pointer_cast< T >( q.front( ) );
			q.pop( );
			//obj->active = true;
			obj->SetActive(true);
			return obj;
		}

		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	void ReturnObject(std::shared_ptr<T> obj) {
		//obj->active = false;
		obj->SetActive(false);
		pool[ std::type_index(typeid( T )) ].push(obj);
	}


};

