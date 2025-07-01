#include <Windows.h>
#include "BaseGame.h"
#include <WinUser.h>

#include "Core/TimerManager.h"
#include "Core/InputManager.h"

#include "Compoment/Component.h"
#include "Compoment/Transform.h"
#include "UICompoment/Button.h"

BaseGame::~BaseGame()
{
    if (m_pTimerManager)
    {
		delete m_pTimerManager;
        m_pTimerManager = nullptr;
    }

    if (m_pInputManager)
    {
		delete m_pInputManager;
        m_pInputManager = nullptr;
    }
}

void BaseGame::Init()
{
	m_pTimerManager = new TimerManager();
	m_pInputManager = new InputManager();


    Test_InitScene();
}

void BaseGame::Run() {
    currentScene.Start();

    MSG msg = {};
    while (isRunning) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) isRunning = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //currentScene.Update();

        // 렌더링 호출 등
    }
}

void BaseGame::CleanUp()
{
}

void BaseGame::UpdateTimer()
{
    // 타이머 업데이트 적용

    m_pTimerManager->Update();
}

void BaseGame::UpdateInput(UINT message, WPARAM wParam, LPARAM lParam)
{
    // 타이머및 인풋쪽 값들
    //m_pInputManager->MouseMove(lParam);

    switch (message) {
    case WM_KEYDOWN:
        InputManager::KeyDown(wParam);
        break;
    case WM_KEYUP:
        InputManager::KeyUp(wParam);
        break;
    case WM_MOUSEMOVE:
        InputManager::MouseMove(lParam);
        break;
    case WM_LBUTTONDOWN:
        InputManager::MouseDown(wParam);
        break;
    case WM_LBUTTONUP:
        InputManager::MouseUp(wParam);
        break;
    }
}

void BaseGame::Update()
{
    currentScene.Update( m_pTimerManager->GetDeltaTime() );
}
void BaseGame::Render(HDC p_hdc)
{
	currentScene.Render(p_hdc);
}

void BaseGame::Test_InitScene()
{
	GameObject* bunobj = currentScene.CreateObject("TestObject");

    bunobj->AddComponent< Transform>();
    bunobj->AddComponent< Button>();
 //   bunobj->AddComponent< Button>([](Button* pButton) 
 //   {
 //   // 버튼 클릭시 동작
 //   MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
	//});
}
