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


class ImageManager : public SingletonT<ImageManager> {
private:
    std::unordered_map<std::wstring, Gdiplus::Image*> imageMap;

public:
    ~ImageManager();

    Gdiplus::Image* Load(const std::wstring& path);
};