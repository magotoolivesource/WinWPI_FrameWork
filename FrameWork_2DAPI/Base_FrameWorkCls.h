#pragma once
#include "Scene/Scene.h"
#include <string>

class TimerManager; // 전방 선언
class InputManager;
class UtilTimer;

class GameObject;
class RigidbodyComponent;
class Test_PlayerMoveCom;


class Base_FrameWorkCls
{
public:
	Base_FrameWorkCls();
    virtual ~Base_FrameWorkCls();


public:
	virtual int InGameMessageLoopWile( );

	virtual LRESULT InGameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    Scene* m_CurrentScene = nullptr;
    bool isRunning = true;

    virtual void Init(HWND p_hwnd, HACCEL p_haccel);
	virtual void Release();
	virtual void Run();
	virtual void CleanUp();

	virtual void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // 예: 입력 처리
	virtual void AllUpdate(); // 예: 게임 상태 갱신

	virtual void Render(HDC p_hdc, RECT& p_clientRect);   // 예: 그리기

	virtual void UpdateDebugEnd();

	virtual void SetHwnd(HWND hwnd) { m_Hwnd = hwnd; } // 윈도우 핸들 설정
	virtual void SetWindowSize(HWND hwnd);

protected:
	virtual void UpdateInputReset();
	virtual void UpdateTimer(); // 예: 입력 처리

	virtual void Update(); // 예: 게임 상태 갱신


protected:
	virtual void InitConsoleWindow();

protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

	COLORREF m_BGColor = RGB(255, 255, 255); // 배경색 초기화 (검정색)

    ULONG_PTR gdiplusToken;
	virtual void InitGDIPlus(); // GDI+ 초기화 함수
	virtual void ReleaseGDIPlus(); // GDI+ 해제 함수

	HWND m_Hwnd = nullptr;
	HACCEL m_hAccelTable = nullptr;

	bool m_isRunning = true; // 게임 루프를 제어하는 변수
public:
    
};

