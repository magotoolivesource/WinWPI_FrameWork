#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>


class GameObject;
class Transform;
class Component abstract // Base class for all components
{
public:
    //virtual void Start() {}
    //virtual void Update() {}
    //GameObject* gameObject = nullptr;
    //virtual ~Component() = default;
	Component( ) = default;
	virtual ~Component( ) {}


    GameObject* owner = nullptr;
    virtual void Start() {
		m_ISGameObjectAddInit = true;

	}
    virtual void Update(float dt) {}
    virtual void Render(HDC hdc) {}
    virtual void Initialize_AddCompoment();
	virtual void RemoveCompoment( );
    virtual void Initialize(){}
	virtual void Release() {}
    virtual void Destroy() {}

public:
    void SetEnabled(bool e) { enabled = e; }
    bool IsEnabled() const { return enabled; }

protected:
    bool enabled = true;
	bool m_ISDestroyIng = false;

public:
	bool GetISDestroy( ) { return m_ISDestroyIng; }

public:
    Transform* transform = nullptr;


protected:
	bool m_ISGameObjectAddInit = false;
public:
	bool ISGameObjectAddInit( ) { return m_ISGameObjectAddInit; }
};

