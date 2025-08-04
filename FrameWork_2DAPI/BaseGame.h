#pragma once
#include "Scene/Scene.h"
#include <string>

class TimerManager; // 전방 선언
class InputManager;
class UtilTimer;

class GameObject;
class RigidbodyComponent;
class Test_PlayerMoveCom;


class BaseGame
{
public:
    BaseGame();
    virtual ~BaseGame();

public:
    Scene* m_CurrentScene = nullptr;
    bool isRunning = true;

    void Init(HWND p_hwnd);
    void Release();
    void Run();
    void CleanUp();

	void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // 예: 입력 처리
    void AllUpdate(); // 예: 게임 상태 갱신

    void Render(HDC p_hdc, RECT& p_clientRect);   // 예: 그리기

	void UpdateDebugEnd();

	void SetHwnd(HWND hwnd) { m_Hwnd = hwnd; } // 윈도우 핸들 설정
    void SetWindowSize(HWND hwnd);

protected:
    void UpdateInputReset();
    void UpdateTimer(); // 예: 입력 처리

    void Update(); // 예: 게임 상태 갱신


protected:
	void InitConsoleWindow();

protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

	COLORREF m_BGColor = RGB(255, 255, 255); // 배경색 초기화 (검정색)

    ULONG_PTR gdiplusToken;
	void InitGDIPlus(); // GDI+ 초기화 함수
    void ReleaseGDIPlus(); // GDI+ 해제 함수

	HWND m_Hwnd = nullptr;


public:
    void Test_InitScene();
    void Test_Image();
    void Test_Button();
    void Test_TextLabel();

	void Test_Collider();

	void Test_TimerLoger( );


	void Test_AniCallBackFN1(UtilTimer* utiltimer, void* p_data);
	void Test_AniCallBackFN(UtilTimer* utiltimer, void* p_data);


	void Test_RigidBody( );
	void Test_RigidBoxCollider( float p_x, float p_y, std::string p_objname );

protected:
	GameObject* m_playerbody;
	Test_PlayerMoveCom* m_TestPlayerMoveCom;
};

