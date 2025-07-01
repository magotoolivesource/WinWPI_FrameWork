#pragma once
#include <windows.h>
#include <cstring>

class InputManager {
public:
    static bool keys[256];
    static bool mouseLeftDown;
    static POINT mousePosition;

    static void Init() {
        memset(keys, 0, sizeof(keys));
        mouseLeftDown = false;
        mousePosition = {0, 0};
    }

    static void KeyDown(WPARAM key) {
        if (key < 256) keys[key] = true;
    }

    static void KeyUp(WPARAM key) {
        if (key < 256) keys[key] = false;
    }

    static void MouseMove(LPARAM lParam) {
        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
    }

    static void MouseDown(WPARAM btn) {
        if (btn & MK_LBUTTON) mouseLeftDown = true;
    }

    static void MouseUp(WPARAM btn) {
        if (!(btn & MK_LBUTTON)) mouseLeftDown = false;
    }

    static bool IsKeyPressed(int key) {
        return keys[key];
    }

    static POINT GetMousePosition() {
        return mousePosition;
    }

    static bool IsMouseLeftDown() {
        return mouseLeftDown;
    }
};

//bool InputManager::keys[256];
//bool InputManager::mouseLeftDown = false;
//POINT InputManager::mousePosition = {0, 0};