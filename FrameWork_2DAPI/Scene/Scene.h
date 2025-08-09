#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

//class GameObject;
#include "../Core/GameObject.h"


enum class E_MARKDIRTYTYPE
{
	SORTING,
	TRANSFORM,

};


// 
class Scene
{
public:
	virtual void PrevInitSettings( ) {};
	virtual void InitSettings( ) {};


	virtual void MarkDirty( ) { isDirty = true;	}
	virtual void UpdateIfDirty( );
protected:
    std::vector<std::unique_ptr<GameObject>> m_AllOjects;
	std::vector<GameObject*> m_sortedObjects;

	bool isDirty = true;
public:
    GameObject* CreateObject(const std::string& name) {
        auto obj = std::make_unique<GameObject>();
        //obj->name = name;
        obj->SetName(name);
		obj->InitCreateScene(this);

        GameObject* ptr = obj.get();
        m_AllOjects.emplace_back(std::move(obj));
		m_sortedObjects.emplace_back(ptr);
        return ptr;
    }

	void DestroyObject(GameObject* obj) {

		// for(auto item : objects )
		for(size_t i = 0; i < m_AllOjects.size(); i++) {
			if (m_AllOjects[i].get() == obj) {

				m_sortedObjects.erase(m_sortedObjects.begin( ) + i);
				m_AllOjects[i].release(); // Release the unique_ptr
				m_AllOjects.erase(m_AllOjects.begin() + i);
				return;
			}
		}

	}

    void Start() 
    {
        for (auto& obj : m_AllOjects) 
            obj->Start();
    }

	void Update(float dt);

	void Render(HDC p_hdc);

    void Release() 
    {
        for (size_t i = 0; i < m_AllOjects.size(); i++)
        {
			m_AllOjects[i].release();
        }

		m_AllOjects.clear( );
		m_sortedObjects.clear( );
	}
};

