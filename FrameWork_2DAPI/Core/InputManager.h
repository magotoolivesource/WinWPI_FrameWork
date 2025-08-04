#pragma once
#include <windows.h>
#include <cstring>


enum class InputKey : int {
	None = 0,
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,
	Cancel = VK_CANCEL,
	MiddleMouse = VK_MBUTTON,

	Backspace = VK_BACK,
	Tab = VK_TAB,
	Enter = VK_RETURN,
	Shift = VK_SHIFT,
	Control = VK_CONTROL,
	Alt = VK_MENU,
	Escape = VK_ESCAPE,
	Space = VK_SPACE,

	LeftArrow = VK_LEFT,
	UpArrow = VK_UP,
	RightArrow = VK_RIGHT,
	DownArrow = VK_DOWN,

	// 대문자 알파벳 (WinAPI에 존재하는 키값)
	A = 'A', B = 'B', C = 'C', D = 'D', E = 'E',
	F = 'F', G = 'G', H = 'H', I = 'I', J = 'J',
	K = 'K', L = 'L', M = 'M', N = 'N', O = 'O',
	P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T',
	U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y', Z = 'Z',

	// 소문자 (실제 키 입력은 없음, 구분용 가상 키값 1000부터 시작)
	//a = 'a', b, c, d, e,
	//f, g, h, i, j,
	//k, l, m, n, o,
	//p, q, r, s, t,
	//u, v, w, x, y, z,
	a = 'a', b = 'b', c = 'c', d = 'd', e = 'e',
	f = 'f', g = 'g', h = 'h', i = 'i', j = 'j',
	k = 'k', l = 'l', m = 'm', n = 'n', o = 'o',
	p = 'p', q = 'q', r = 'r', s = 's', t = 't',
	u = 'u', v = 'v', w = 'w', x = 'x', y = 'y', z = 'z',

	Num0 = '0',
	Num1 = '1',
	Num2 = '2',
	Num3 = '3',
	Num4 = '4',
	Num5 = '5',
	Num6 = '6',
	Num7 = '7',
	Num8 = '8',
	Num9 = '9',
};

enum class E_MOUSEETYPE
{
	LEFT,
	RIGHT,
	MIDDLE,

	MAX
};

// https://chatgpt.com/c/685d15a6-5204-8013-ae97-2bd6dfe11517
// https://chatgpt.com/c/68900e64-2570-8013-bcce-67080d0adf94
class InputManager {
	static const bool IS_OnlyUpperKeyPress = true;
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

			if ( IS_OnlyUpperKeyPress
				&& key >= 'a' && key <= 'z' )
			{
				// 대소문자 가릴지 여부용
				key -= 32;// 'a' - 'A';
			}
			
			// 대소문자 가릴지 여부용
			keys[ key ] = true;
			iskeys_down[ key ] = true;
        }
    }

    static void KeyUp(WPARAM key) {
        if (key < 256) 
		{
			if ( IS_OnlyUpperKeyPress
				&& key >= 'a' && key <= 'z' )
			{
				// 대소문자 가릴지 여부용
				key -= 32;// 'a' - 'A';
			}

            keys[key] = false;
            //iskeys_pressed[key] = true;
            iskeys_up[key] = true;
        }
    }

	

	// 눌렀을때 trigger 방식
	static bool ISKeyDown(int key) 
	{ 
		return iskeys_down[key];
    }
	static bool ISKeyDown(InputKey key)
	{
		return iskeys_down[ (int)key ];
	}

	// 눌렀다 땠을때 trigger 방식
	static bool ISKeyUp(int key) 
	{ 
		return iskeys_up[key];
    }
	static bool ISKeyUp(InputKey key)
	{
		return iskeys_up[(int)key ];
	}

    /// <summary>
    /// 누루고 있을때 호출적용
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    static bool IsKeyPressed(int key) 
	{ 
		return keys[key]; 
	}
	static bool IsKeyPressed(InputKey p_key)
	{
		return keys[ (int)p_key ];
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