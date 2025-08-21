#include "ZombieFactoryManager.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Compoment/ImageComponent.h>
#include <UICompoment/Button.h>
#include <Core/UtilLoger.h>

#include "InGameDefineDatas.h"
#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"
#include "Base_ZombiActor.h"


using namespace std;


unsigned int ZombieFactoryManager::m_UID = 0;

//
//ZombieFactoryManager::ZombieFactoryManager( )
//{
//}
//
//ZombieFactoryManager::~ZombieFactoryManager( )
//{
//
//}
//
//void ZombieFactoryManager::RegisterZombie(const std::string& typeName, Creator creator)
//{
//	creators[ typeName ] = creator;
//}
//
//std::unique_ptr<Base_ZombiActor> ZombieFactoryManager::CreateZombie(const std::string& typeName)
//{
//	auto it = creators.find(typeName);
//	if ( it != creators.end( ) ) {
//		return it->second( );
//	}
//	return nullptr;
//}



