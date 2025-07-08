#pragma once
#include <vector>
#include <memory>
#include <string>

//class GameObject;
#include "../Core/GameObject.h"


class Scene
{
public:
    std::vector<std::unique_ptr<GameObject>> objects;

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
		for(size_t i = 0; i < objects.size(); i++) {
			if (objects[i].get() == obj) {
				objects[i].release(); // Release the unique_ptr
				objects.erase(objects.begin() + i);
				return;
			}
		}

	}

    void Start() 
    {
        for (auto& obj : objects) 
            obj->Start();
    }

    void Update(float dt)
    {
        for (auto& obj : objects) 
            obj->Update(dt);
    }

    void Render(HDC p_hdc) 
    {
		//std::vector<GameObject*> sortedObjects(objects.begin( ), objects.end( ));
		//std::sort(sortedObjects.begin( ), sortedObjects.end( ), [ ] (GameObject* a, GameObject* b) {
		//	return a->GetTransform( )->GetWorldDepth( ) < b->GetTransform( )->GetWorldDepth( );
		//	});

        for (auto& obj : objects) 
            obj->Render(p_hdc);
	}

    void Release() 
    {
        for (size_t i = 0; i < objects.size(); i++)
        {
			objects[i].release();
        }
	}
};

