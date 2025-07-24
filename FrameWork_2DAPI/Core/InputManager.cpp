//#pragma once
#include "InputManager.h"


bool InputManager::keys[256];
bool InputManager::mouseLeftDown = false;
bool InputManager::mouseRightDown = false;
bool InputManager::mouseMiddleDown = false;
POINT InputManager::mousePosition = { 0, 0 };
bool InputManager::iskeys_down[256];
//bool InputManager::iskeys_pressed[256];
bool InputManager::iskeys_up[256];

bool InputManager::ismouse_down[(int)E_MOUSEETYPE::MAX] = { 0, };
bool InputManager::ismouse_up[(int)E_MOUSEETYPE::MAX] = { 0, };