#pragma once
#include "Scene/Scene.h"


class TimerManager; // ���� ����
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

    void UpdateTimer(); // ��: �Է� ó��
	void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // ��: �Է� ó��
    void Update();   // ��: ���� ���� ����
    void Render(HDC p_hdc, RECT& p_clientRect);   // ��: �׸���


protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

	COLORREF m_BGColor = RGB(255, 255, 255); // ���� �ʱ�ȭ (������)

public:
    void Test_InitScene();


};

