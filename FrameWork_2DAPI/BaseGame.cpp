#include <Windows.h>
#include "BaseGame.h"
#include <WinUser.h>
#include <gdiplus.h>

#include "Core/TimerManager.h"
#include "Core/InputManager.h"

#include "Compoment/Component.h"
#include "Compoment/Transform.h"
#include "Compoment/ImageComponent.h"
#include "UICompoment/Button.h"
#include "UICompoment/TextComponent.h"
#include "UICompoment/RichTextComponent.h"


using namespace Gdiplus;

BaseGame::BaseGame()
{
	m_CurrentScene = new Scene();
}

BaseGame::~BaseGame()
{
	Release();
}

void BaseGame::Init()
{
    InitGDIPlus();
    
	m_pTimerManager = new TimerManager();
	m_pInputManager = new InputManager();


    Test_InitScene();
}

void BaseGame::Release()
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Release();
        delete m_CurrentScene;
        m_CurrentScene = nullptr;
    }

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


    ReleaseGDIPlus();
}

void BaseGame::Run() {
    m_CurrentScene->Start();

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
    m_CurrentScene->Update( m_pTimerManager->GetDeltaTime() );
}
void BaseGame::Render(HDC p_hdc, RECT& p_clientRect)
{
    // 2. 배경 지우기 (하얀색)
    HBRUSH bg = CreateSolidBrush(m_BGColor );
    FillRect(p_hdc, &p_clientRect, bg);
    DeleteObject(bg);

	m_CurrentScene->Render(p_hdc);
}

void BaseGame::InitGDIPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void BaseGame::ReleaseGDIPlus()
{
    GdiplusShutdown(gdiplusToken);
}



void BaseGame::Test_InitScene()
{
	
    // 버턴
    GameObject* bunobj = m_CurrentScene->CreateObject("TestObject");
    bunobj->AddComponent< Button>();
    bunobj->AddComponent<Button>([](Button* p_owerbtn) 
    {
        // 버튼 클릭시 동작
        MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
		});
    bunobj->GetComponent<Transform>()->setPosition(50, 100);


    // 이미지
	GameObject* pngimgobj = m_CurrentScene->CreateObject("PngImageObject");
	//pngimgobj->AddComponent<ImageComponent>();
    pngimgobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    pngimgobj->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/Mobile - Final Fantasy Record Keeper - Elarra Eiko Model.png");


    // 문자
    GameObject* textobj = m_CurrentScene->CreateObject("Text");
    TextComponent* textcom = textobj->AddComponent<TextComponent>();
    textcom->SetText(L"Game Start!가나다");
    textcom->SetFontColor(100, 255, 0, 0); // 반투명 흰색
    textcom->SetFont(L"Consolas", 28);
	textobj->transform->setPosition(200, 100);

	// richitext 컴포넌트 테스트용
	GameObject* richtextobj = m_CurrentScene->CreateObject("RichText");
	RichTextComponent* richText = richtextobj->AddComponent<RichTextComponent>();
	//richText.SetTransform(&transform);
	richText->SetText(L"기본 텍스트\\n<color=#FFFF0000>빨강</color> <color=#FF00FF00>초록</color>");
	richText->SetFont(L"Consolas", 28);
	richText->SetSize(400, 200);
	richText->SetAlignment(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);
	richText->SetStyle(true, false, true); // Bold + Underline
	richText->SetOutline(true, 255, 0, 255, 0); // 그린 테두리

	richtextobj->transform->setPosition(200, 300);


    //bunobj->AddComponent< Transform>();

    //bunobj->AddComponent< Transform>();
    
 //   bunobj->AddComponent< Button>([](Button* pButton) 
 //   {
 //   // 버튼 클릭시 동작
 //   MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
	//});
}
