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

#include "Manager/FacadeManager.h"


#include "Manager/CameraManager.h"
#include "Compoment/Camera.h"


#include <cassert>

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
    FacadeManager::GetI()->Initlize(); // Initialize FacadeManager

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

	FacadeManager::GetI()->DestroyManager();
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
	


	
    // 버턴 1
    GameObject* bunobj = m_CurrentScene->CreateObject("TestObject");
    bunobj->AddComponent< Button>();
    bunobj->AddComponent<Button>([](Button* p_owerbtn) 
    {
        // 버튼 클릭시 동작
        MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
		});
    bunobj->GetComponent<Transform>()->setLocalPosition(50, 100);

	// 버턴 2
	GameObject* bunobj2 = m_CurrentScene->CreateObject("TestObject2");
	bunobj2->AddComponent<Button>([ ] (Button* p_owerbtn)
		{
			// 버튼 클릭시 동작
			MessageBox(nullptr, L"버턴2!", L"Info", MB_OK);
		});
	bunobj2->GetComponent<Transform>( )->setLocalPosition(150, -20);

	bunobj2->GetComponent<Transform>( )->SetParent(bunobj->GetComponent<Transform>( )); // 부모 설정



	// 이미지 2
    GameObject* pngimgobj2 = m_CurrentScene->CreateObject("PngImageObject2");
    pngimgobj2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    pngimgobj2->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/UVTexture.png");
    pngimgobj2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
    pngimgobj2->transform->SetWorldRotation(-45.f); // 월드 회전 설정
    pngimgobj2->transform->SetWorldPosition(150, 200);
    pngimgobj2->transform->SetWorldScale( 0.5f, 0.5f ); // 월드 스케일 설정	

	// 이미지 3
    GameObject* pngimgobj3 = m_CurrentScene->CreateObject("PngImageObject3");
    pngimgobj3->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    pngimgobj3->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/UVTexture.png");
    pngimgobj3->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
    pngimgobj3->transform->SetWorldRotation(45.f); // 월드 회전 설정
    pngimgobj3->transform->SetWorldPosition(150, 200);
    //pngimgobj3->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정	
	pngimgobj3->transform->SetParent(pngimgobj2->transform); // 부모 설정


    // 이미지 1
	GameObject* pngimgobj = m_CurrentScene->CreateObject("PngImageObject");
	//pngimgobj->AddComponent<ImageComponent>();
    pngimgobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    //pngimgobj->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/Mobile - Final Fantasy Record Keeper - Elarra Eiko Model.png");
	pngimgobj->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/UVTexture.png");
	pngimgobj->transform->SetWorldPosition(150, 200);
    pngimgobj->GetComponent<ImageComponent>()->SetEnabled(false);

	


    // 문자
    GameObject* textobj = m_CurrentScene->CreateObject("Text");
    TextComponent* textcom = textobj->AddComponent<TextComponent>();
    textcom->SetText(L"Game Start!가나다");
    textcom->SetFontColor(100, 255, 0, 0); // 반투명 흰색
    textcom->SetFont(L"Consolas", 28);
	textobj->transform->setLocalPosition(200, 100);

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

	richtextobj->transform->setLocalPosition(200, 300);



	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
	mainCamera->SetWorldPosition(0, 0);
    mainCamera->SetZoom(0.5f); // 카메라 줌 설정
	mainCamera->SetWorldRotation(-45.f); // 카메라 회전 설정


    //bunobj->AddComponent< Transform>();

    //bunobj->AddComponent< Transform>();
    
 //   bunobj->AddComponent< Button>([](Button* pButton) 
 //   {
 //   // 버튼 클릭시 동작
 //   MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
	//});



	//assert(false && "이 메시지는 NDEBUG가 정의되지 않았을 때만 보입니다.");

	//exit(0); // 프로그램 종료


}
