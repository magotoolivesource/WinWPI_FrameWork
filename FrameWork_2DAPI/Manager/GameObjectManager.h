#pragma once
//#include <windows.h>
//#include <gdiplus.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>
#include <type_traits>
#include <string>

#include "../Core/SingletonT.h"
#include "../Core/GameObject.h"


//class GameObject; // Forward declaration of GameObject

class GameObjectManager : public SingletonT<GameObjectManager> {

protected:
	//std::unordered_map<std::type_index, std::unique_ptr<GameObject>> m_AllCameraMap;
	std::vector<std::unique_ptr<GameObject>> objects;

public:
    GameObjectManager() = default;
    virtual ~GameObjectManager();

	static GameObject* CreateObjectS(const std::string& name); 
    static void DestroyObjectS(GameObject* obj);

	GameObject* CreateObject(const std::string& name) {
        auto obj = std::make_unique<GameObject>();
        //obj->name = name;
        obj->SetName(name);
        GameObject* ptr = obj.get();
        objects.emplace_back(std::move(obj));
        return ptr;
    }

    void DestroyObject(GameObject* obj) {

        // for(auto item : objects )
        for (size_t i = 0; i < objects.size(); i++) {
            if (objects[i].get() == obj) {
                objects[i].release(); // Release the unique_ptr
                objects.erase(objects.begin() + i);
                return;
            }
        }
    }
	
public:
    
};