#pragma once
//#include <windows.h>
//#include <gdiplus.h>
#include <unordered_map>
#include <string>
#include "../Core/SingletonT.h"

// Forward declaration of Gdiplus::Image to avoid including the entire Gdiplus header
namespace Gdiplus{
    class Image;
};


class PhysicsManager : public SingletonT<PhysicsManager> {
private:

public:
    virtual ~PhysicsManager();

};