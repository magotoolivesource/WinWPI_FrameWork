#pragma once
#include <gdiplus.h>
#include <unordered_map>
#include <memory>
#include <string>

class ImageManager {
private:
    std::unordered_map<std::wstring, std::weak_ptr<Gdiplus::Image>> imageMap;

public:
    std::shared_ptr<Gdiplus::Image> Load(const std::wstring& path) {
        auto it = imageMap.find(path);
        if (it != imageMap.end()) {
            std::shared_ptr<Gdiplus::Image> cached = it->second.lock();
            if (cached) return cached;
        }

        std::shared_ptr<Gdiplus::Image> img(new Gdiplus::Image(path.c_str()));
        if (img && img->GetLastStatus() == Gdiplus::Ok) {
            imageMap[path] = img;
            return img;
        }
        return nullptr;
    }
};