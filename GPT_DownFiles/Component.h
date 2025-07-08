#pragma once

class GameObject;

class Component {
public:
    GameObject* gameObject = nullptr;
    bool enabled = true;

    virtual ~Component() {}

    void SetEnabled(bool e) { enabled = e; }
    bool IsEnabled() const { return enabled; }
};