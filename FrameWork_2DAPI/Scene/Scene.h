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
// //1번 https://gemini.google.com/app/36a5f1ab8451eba4
// // https://chatgpt.com/c/689c15b8-1768-8329-9950-f6f603b91a95
// 1번 https://chatgpt.com/c/689c1c49-4af0-832d-a0c3-f1afc0ea8551
class Scene
{
public:
	virtual void PrevInitSettings( ) {};
	virtual void InitSettings( ) {};



protected:
    std::vector<std::unique_ptr<GameObject>> m_AllOjects;
	std::vector<GameObject*> m_sortedObjects;

	
public:
	GameObject* CreateObject(const std::string& name);

	virtual void DestroyObject(GameObject* obj);

	virtual void Start()
    {
        for (auto& obj : m_AllOjects) 
            obj->Start();
    }

	virtual void UpdateLoop(float dt);

	virtual void Render(HDC p_hdc);

	virtual void Release()
    {
        for (size_t i = 0; i < m_AllOjects.size(); i++)
        {
			m_AllOjects[i].release();
        }
		m_AllOjects.clear( );
		m_sortedObjects.clear( );

		m_pendingObjects.clear( );
		m_pendingStartObjects.clear( );
		m_destroyQueue.clear( );
	}


public:
	void AddRuntimeDirtyAddComponent(GameObject* obj);

protected:
	// 추가
	std::vector<std::unique_ptr<GameObject>> m_pendingObjects;      // 다음 프레임에 추가될 객체
	std::vector<GameObject*> m_pendingStartObjects;                 // Start 호출 대기
	std::vector<GameObject*> m_destroyQueue;                        // 삭제 예약

	std::vector<GameObject*> m_RuntimeAddComponentObjects;                 // Start 호출 대기

	bool isDirty = true;
	//bool isRuntimeAddComponentDirty = true;

public:
	virtual void ProcessNewObjects( );
	virtual void ProcessRunTimeAddComponentObject( );

	virtual void Prev_DestroyQueueObjects_AllRemoveComponent( );
	virtual void ProcessDestroyQueue( );

	virtual void MarkDirty( ) { isDirty = true; }
	virtual void UpdateIfDirty( );

	//virtual void MarkRuntimeAddComponentDirty( ) { isRuntimeAddComponentDirty = true; }
	//virtual void UpdateIfRuntimeAddComponentDirty( );

};

