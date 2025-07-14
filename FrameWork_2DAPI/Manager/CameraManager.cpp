#include <windows.h>
#include <gdiplus.h>

#include "CameraManager.h"
#include "GameObjectManager.h"
#include "../Compoment/Camera.h"


CameraManager::CameraManager() 
{ 
	AddMainCamera(); // 생성자에서 메인 카메라를 추가
}

CameraManager::~CameraManager() {
    for (auto& cam : m_AllCameraVec) {
        cam = nullptr; // 카메라 포인터를 nullptr로 설정
    }

    m_AllCameraVec.clear();
}


Camera* CameraManager::AddCamera(const std::string& name) 
{ 
	auto cam = GameObjectManager::CreateObjectS(name)->AddComponent<Camera>();
    m_AllCameraVec.push_back(cam); // 벡터에 카메라 추가
    return cam;
}

Camera* CameraManager::AddMainCamera() 
{ 
	if (m_MainCamera) 
	{
		return m_MainCamera; // 이미 메인 카메라가 존재하면 반환
    }

	auto cam = GameObjectManager::CreateObjectS("MainCamera")->AddComponent<Camera>();
    m_AllCameraVec.push_back(cam); // 벡터에 카메라 추가
	m_MainCamera = cam; 
	return cam; 
}

Camera* CameraManager::GetMainCamera() 
{ 
	return m_MainCamera;
}

