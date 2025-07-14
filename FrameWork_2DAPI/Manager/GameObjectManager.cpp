#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

#include <type_traits>
#include <string>
//#include "../Core/SingletonT.h"
//#include "../Compoment/Camera.h"
#include "GameObjectManager.h"
#include "../Core/GameObject.h"


GameObjectManager::~GameObjectManager() 
{ 
	for (size_t i = 0; i < objects.size(); i++) {
        objects[i].release();
    }
}

GameObject* GameObjectManager::CreateObjectS(const std::string& name) 
{
    return GameObjectManager::GetI()->CreateObject(name); 
}

void GameObjectManager::DestroyObjectS(GameObject* obj)
{
    GameObjectManager::GetI()->DestroyObject(obj);
}
