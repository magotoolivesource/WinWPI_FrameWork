#pragma once
#include <unordered_map>
#include <string>
#include "../Core/SingletonT.h"


class FacadeManager : public SingletonT<FacadeManager> {

public:
    FacadeManager();
    virtual ~FacadeManager();

	void Initlize();
    void Release();

	void DestroyAllManagers();
};