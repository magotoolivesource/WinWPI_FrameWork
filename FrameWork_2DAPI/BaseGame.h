#pragma once
#include "Scene/Scene.h"


class TimerManager; // 전방 선언
class InputManager;

class BaseGame
{
public:
    virtual ~BaseGame();

public:
    Scene currentScene;
    bool isRunning = true;

    void Init();
    void Run();
    void CleanUp();

    void UpdateTimer(); // 예: 입력 처리
	void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // 예: 입력 처리
    void Update();   // 예: 게임 상태 갱신
    void Render(HDC p_hdc);   // 예: 그리기


protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

public:
    void Test_InitScene();


};

