#pragma once
#include "Scene/Scene.h"


class TimerManager; // 전방 선언
class InputManager;

class BaseGame
{
public:
    BaseGame();
    virtual ~BaseGame();

public:
    Scene* m_CurrentScene;
    bool isRunning = true;

    void Init();
    void Release();
    void Run();
    void CleanUp();

    void UpdateTimer(); // 예: 입력 처리
	void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // 예: 입력 처리
    void Update();   // 예: 게임 상태 갱신
    void Render(HDC p_hdc, RECT& p_clientRect);   // 예: 그리기


protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

	COLORREF m_BGColor = RGB(255, 255, 255); // 배경색 초기화 (검정색)

    ULONG_PTR gdiplusToken;
	void InitGDIPlus(); // GDI+ 초기화 함수
    void ReleaseGDIPlus(); // GDI+ 해제 함수
public:
    void Test_InitScene();


};

