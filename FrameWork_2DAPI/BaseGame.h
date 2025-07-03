#pragma once
#include "Scene/Scene.h"


class TimerManager; // ���� ����
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

    void UpdateTimer(); // ��: �Է� ó��
	void UpdateInput(UINT message, WPARAM wParam, LPARAM lParam); // ��: �Է� ó��
    void Update();   // ��: ���� ���� ����
    void Render(HDC p_hdc, RECT& p_clientRect);   // ��: �׸���


protected:
	TimerManager* m_pTimerManager = nullptr;
	InputManager* m_pInputManager = nullptr;

	COLORREF m_BGColor = RGB(255, 255, 255); // ���� �ʱ�ȭ (������)

    ULONG_PTR gdiplusToken;
	void InitGDIPlus(); // GDI+ �ʱ�ȭ �Լ�
    void ReleaseGDIPlus(); // GDI+ ���� �Լ�
public:
    void Test_InitScene();


};

