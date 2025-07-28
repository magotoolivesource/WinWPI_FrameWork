#include "FacadeManager.h"

#include "CameraManager.h"
#include "GameObjectManager.h"
#include "ImageManager.h"
#include "CollisionManager.h"


FacadeManager::FacadeManager() 
{ 
	Initlize();
}

FacadeManager::~FacadeManager() 
{
	Release();
}

void FacadeManager::Initlize() 
{
    ImageManager::GetI(); // Initialize ImageManager
    GameObjectManager::GetI(); // Initialize GameObjectManager
    CameraManager::GetI(); // Initialize CameraManager
	CollisionManager::GetI();
}

void FacadeManager::Release( )
{
    CameraManager::Instance().DestroyManager();
    GameObjectManager::Instance().DestroyManager();
    ImageManager::Instance().DestroyManager();

}

void FacadeManager::DestroyAllManagers() 
{
    ImageManager::Instance().DestroyManager();
    GameObjectManager::Instance().DestroyManager();
    CameraManager::Instance().DestroyManager();
}
