#pragma once
//#include <windows.h>
//#include <gdiplus.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

#include <type_traits>
#include <string>
#include "../Core/SingletonT.h"
#include "../Core/Vector.h"
//#include "../Compoment/Camera.h"

#include <windows.h>


class Camera;

class CameraManager : public SingletonT<CameraManager> {

protected:
    //std::unique_ptr<Component>
    //std::unordered_map<std::wstring, Gdiplus::Image*> imageMap;
    //vector< Camera*
    //std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    //std::vector<std::unique_ptr<GameObject>> objects;

    //std::unordered_map<std::type_index, std::unique_ptr<Camera>> m_AllCameraMap;
    //   std::unordered_map<std::type_index, Camera*> m_AllCameraMap;


    std::vector<Camera*> m_AllCameraVec;
    Camera* m_MainCamera = nullptr;

public:
    CameraManager();
    virtual ~CameraManager();

    //// Add a camera to the manager
    //template<typename T, typename... Args>
    //T* AddCamera(Args&&... args) {
    //	static_assert(std::is_base_of<Camera, T>::value, "T must be derived from Camera");
    //	auto camera = std::make_unique<T>(std::forward<Args>(args)...);
    //	T* cameraPtr = camera.get();
    //	m_AllCameraMap[std::type_index(typeid(T))] = std::move(camera);
    //	return cameraPtr;
    //}
    //// Get a camera by type
    //template<typename T>
    //T* GetCamera() {
    //	auto it = m_AllCameraMap.find(std::type_index(typeid(T)));
    //	return (it != m_AllCameraMap.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
    //       }


    Camera* AddCamera(const std::string& name = "");

    Camera* AddMainCamera();
    Camera* GetMainCamera();



protected:
    HWND m_hWnd = nullptr;
    Vec2 m_WindowSize = { 800, 600 }; // Default viewport size
    Vec2 m_ViewportCenter = { 800 * 0.5f, 600 * 0.5f }; // Viewport size in pixels

	void UpdateViewportSize();
public:
    void SetWindowSize(HWND hwnd);
    //void SetHwnd(HWND hwnd);
    Vec2& GetWindowSize() { return m_WindowSize; }
    Vec2& GetViewportCenter() { return m_ViewportCenter; }

};
