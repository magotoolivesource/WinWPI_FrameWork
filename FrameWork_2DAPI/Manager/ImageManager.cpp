#include "ImageManager.h"
#include <windows.h>
#include <gdiplus.h>

//#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib")


ImageManager::~ImageManager()
{
    for (auto& pair : imageMap) {
        delete pair.second;
    }
    imageMap.clear();
}

Gdiplus::Image* ImageManager::Load(const std::wstring& path)
{
    auto it = imageMap.find(path);
    if (it != imageMap.end()) return it->second;

    Gdiplus::Image* img = new Gdiplus::Image(path.c_str());
    if (img && img->GetLastStatus() == Gdiplus::Ok) {
        imageMap[path] = img;
        return img;
    }
    delete img;
    return nullptr;
}