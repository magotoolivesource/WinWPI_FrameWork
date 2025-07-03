#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>

class GameObject;
class Component
{
public:
    //virtual void Start() {}
    //virtual void Update() {}
    //GameObject* gameObject = nullptr;
    //virtual ~Component() = default;

    GameObject* owner = nullptr;
    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void Render(HDC hdc) {}
    virtual ~Component() {}
    virtual void Initialize_AddCompoment() {}
    virtual void Initialize(){}
	virtual void Release() {}
    virtual void Destroy() {}

public:
    bool enabled = true;
    void SetEnabled(bool e) { enabled = e; }
    bool IsEnabled() const { return enabled; }

};

