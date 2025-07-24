#pragma once
#include <windows.h>
#include <cstring>


enum class E_MOUSEETYPE
{
	LEFT,
	RIGHT,
	MIDDLE,

	MAX
};

class InputManager {
public:
    static bool keys[256];
    static bool mouseLeftDown;
    static bool mouseRightDown;
    static bool mouseMiddleDown;
    static POINT mousePosition;

	static bool ismouse_down[(int)E_MOUSEETYPE::MAX];
    static bool ismouse_up[(int)E_MOUSEETYPE::MAX];

	static bool iskeys_down[256];
    //static bool iskeys_pressed[256];
    static bool iskeys_up[256];

	static void ResetAllKey()
	{ 
		memset(iskeys_down, 0, sizeof(iskeys_down));
        //memset(iskeys_pressed, 0, sizeof(iskeys_pressed));
        memset(iskeys_up, 0, sizeof(iskeys_up));
		memset(ismouse_down, 0, sizeof(ismouse_down));
        memset(ismouse_up, 0, sizeof(ismouse_up));
	}

    static void Init() {
        memset(keys, 0, sizeof(keys));
        memset(iskeys_down, 0, sizeof(iskeys_down));
        memset(iskeys_up, 0, sizeof(iskeys_up));
        memset(ismouse_down, 0, sizeof(ismouse_down));
        memset(ismouse_up, 0, sizeof(ismouse_up));

        mouseLeftDown = false;
        mouseRightDown = false;
        mouseMiddleDown = false;
        mousePosition = {0, 0};
    }

    static void KeyDown(WPARAM key) {
        if (key < 256) {
            keys[key] = true;
            iskeys_down[key] = true;
        }
    }

    static void KeyUp(WPARAM key) {
        if (key < 256) 
		{
            keys[key] = false;
            //iskeys_pressed[key] = true;
            iskeys_up[key] = true;
        }
    }

	static bool ISKeyDown(int key) 
	{ 
		return iskeys_down[key];
    }
	static bool ISKeyUp(int key) 
	{ 
		return iskeys_up[key];
    }
    static bool IsKeyPressed(int key) 
	{ 
		return keys[key]; 
	}

    static void MouseMove(LPARAM lParam) {
        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
    }

    static void MouseDown(UINT message, WPARAM btn) {
        if (btn & MK_LBUTTON) {
            mouseLeftDown = true;
            ismouse_down[(int)E_MOUSEETYPE::LEFT] = true;
        }
        if (btn & MK_RBUTTON) {
            mouseRightDown = true;
            ismouse_down[(int)E_MOUSEETYPE::RIGHT] = true;
        }
        if (btn & MK_MBUTTON) {
            mouseMiddleDown = true;
            ismouse_down[(int)E_MOUSEETYPE::MIDDLE] = true;
        }
    }

    static void MouseUp(UINT message, WPARAM btn) {
        if ((message & WM_LBUTTONUP) == WM_LBUTTONUP)
		{
            mouseLeftDown = false;
            ismouse_up[(int)E_MOUSEETYPE::LEFT] = true;
		}

        if ( (message & WM_RBUTTONUP) == WM_RBUTTONUP ) {
            mouseRightDown = false;
            ismouse_up[(int)E_MOUSEETYPE::RIGHT] = true;
        }

        if ((message & WM_MBUTTONUP) == WM_MBUTTONUP) {
            mouseMiddleDown = false;
            ismouse_up[(int)E_MOUSEETYPE::MIDDLE] = true;
        }

        //if (!(btn & MK_LBUTTON)) {
        //    mouseLeftDown = false;
        //    ismouse_up[(int)E_MOUSEETYPE::LEFT] = true;
        //}
        //if (!(btn & MK_RBUTTON)) {
        //    mouseRightDown = false;
        //    ismouse_up[(int)E_MOUSEETYPE::RIGHT] = true;
        //}
        //if (!(btn & MK_MBUTTON)) {
        //    mouseMiddleDown = false;
        //    ismouse_up[(int)E_MOUSEETYPE::MIDDLE] = true;
        //}
    }

    

    static POINT GetMousePosition() {
        return mousePosition;
    }

    static bool IsMouseLeftDown() {
        return mouseLeftDown;
    }

	static bool ISMouseUp(E_MOUSEETYPE btn) {
		return ismouse_up[(int)btn];
	}
	static bool ISMouseDown(E_MOUSEETYPE btn) {
		return ismouse_down[(int)btn];
	}

};

//bool InputManager::keys[256];
//bool InputManager::mouseLeftDown = false;
//POINT InputManager::mousePosition = {0, 0};