#include <Windows.h>
#include "BaseGame.h"
#include <WinUser.h>
#include <gdiplus.h>
#include <string>
#include <format>
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <cassert>

#include "Core/TimerManager.h"
#include "Core/InputManager.h"


#include "Compoment/Component.h"
#include "Compoment/Transform.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/Camera.h"
#include "Compoment/LineComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Compoment/Collider.h"
#include "Compoment/RigidbodyComponent.h"

#include "UICompoment/Button.h"
#include "UICompoment/TextComponent.h"
#include "UICompoment/RichTextComponent.h"

#include "Manager/FacadeManager.h"
#include "Manager/DebugObjectManager.h"
#include "Manager/CameraManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/PhysicsManager.h"

#include "Core/MyUtil.h"
#include "Core/UtilTimerManager.h"
#include "Core/UtilLoger.h"

#include "z_SampleSource/Test_PlayerMoveCom.h"


using namespace Gdiplus;

GameObject* g_centerobj = nullptr;
ImageComponent* g_tempimgcomp = nullptr;

Collider* g_colliderbox1 = nullptr;
Collider* g_colliderbox2 = nullptr;


void TestCameraAni(BaseGame* p_game)
{
	// 카메라 세팅
	static float cam_anirot = 0.3f; // 0.3f; // 0.5f;
	static float cam_anizoom = 0.f; // 0.01f; // 0.005f;

	static Vec2 cameraPos = { 0, 0 };
	static float camangle = 0.0f;
	static float camzoom = 1.f;
	static bool iscamcenterpos = true;

	if ( iscamcenterpos ) {
		cameraPos.x = -CameraManager::GetI( )->GetViewportCenter( ).x;
		cameraPos.y = -CameraManager::GetI( )->GetViewportCenter( ).y;
	}

	camangle += cam_anirot;
	if ( camangle > 360.f )
	{
		camangle -= 360.f; // 각도 360도 이상으로 가지 않도록
	}
	if ( camangle < -360.f ) {
		camangle += 360.f; // 각도 360도 이상으로 가지 않도록
	}

	camzoom += cam_anizoom;
	if ( camzoom < 0.1f ) {
		cam_anizoom = -cam_anizoom;
	}

	if ( camzoom > 2.0f )
	{
		cam_anizoom = -cam_anizoom;
	}

	Camera* mainCamera = CameraManager::GetI( )->GetMainCamera( );
	mainCamera->SetWorldPosition(cameraPos.x, cameraPos.y);
	mainCamera->SetWorldRotation(camangle);
	mainCamera->SetZoom(camzoom); // 줌 설정

}

void TestImageAni(BaseGame* p_game)
{
	static Vec2 wpos = { 0, 0 };
	static Vec2 rect = { 64, 64 };
	static Vec2 size = { 32, 32 };

	static float imgangle = 0.0f;
	static float anirot2 = -0.6f; //    -0.6f;
	imgangle += anirot2;
	if ( imgangle > 360.f ) {
		imgangle -= 360.f; // 각도 360도 이상으로 가지 않도록
	}
	if ( imgangle < -360.f ) {
		imgangle += 360.f; // 각도 360도 이상으로 가지 않도록
	}

	g_centerobj->GetComponent<ImageComponent>( )->SetDrawRect(0, 0, rect.x, rect.y);
	g_centerobj->GetComponent<ImageComponent>( )->SetSize(size.x, size.y);
	g_centerobj->transform->SetWorldPosition(wpos.x, wpos.y);
	g_centerobj->transform->SetWorldRotation(imgangle);
}

void TestColliderAni(BaseGame* p_game)
{

	static Vec2 wpos = { 0, 0 };
	static Vec2 rect = { 64, 64 };
	static Vec2 size = { 32, 32 };

	static float imgangle = 0.0f;
	static float anirot2 = -0.6f; //    -0.6f;
	imgangle += anirot2;
	if ( imgangle > 360.f ) {
		imgangle -= 360.f; // 각도 360도 이상으로 가지 않도록
	}
	if ( imgangle < -360.f ) {
		imgangle += 360.f; // 각도 360도 이상으로 가지 않도록
	}



}

void TestUpdateFN(BaseGame* p_game)
{
	return;

	TestImageAni(p_game);
	TestCameraAni(p_game);
	TestColliderAni(p_game);

}


void BaseGame::Test_AniCallBackFN1(UtilTimer* utiltimer, void* p_data)
{
	UtilLoger::Log(L"콜백 호출 : ");
}

void BaseGame::Test_AniCallBackFN(UtilTimer* utiltimer, void* p_data)
{
	//g_colliderbox1
	UtilLoger::ClearAll( );
}




BaseGame::BaseGame()
    : gdiplusToken(0) 
{
	m_CurrentScene = new Scene();
}

BaseGame::~BaseGame()
{
	Release();
}



void BaseGame::Init(HWND p_hwnd) 
{
    InitConsoleWindow(); 
    
    InitGDIPlus();

    m_Hwnd = p_hwnd;
    FacadeManager::GetI()->Initlize(); // Initialize FacadeManager
	CameraManager::GetI()->SetWindowSize(p_hwnd); // Set the window handle for CameraManager

	m_pTimerManager = new TimerManager();
	m_pInputManager = new InputManager();

	

    Test_InitScene();
}

void BaseGame::InitConsoleWindow() 
{
    AllocConsole(); // 콘솔창 생성
    //SetConsoleOutputCP(CP_UTF8); // UTF-8 지원

	setlocale(LC_ALL, "korean");
    _wsetlocale(LC_ALL, L"korean");

	FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    //// ✅ UTF-16 출력 모드
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //_setmode(_fileno(stderr), _O_U16TEXT);
    //_setmode(_fileno(stdin), _O_U16TEXT);

    std::ios::sync_with_stdio(true);
    std::wcout.clear();

    std::wcout << L"[Console Initialized] 초기화\n" << std::endl;
    std::wcout.flush();
    
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


	FacadeManager::GetI()->DestroyManager();

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

	UtilTimerManager::GetI()->Update(m_pTimerManager->GetDeltaTime());

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
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        InputManager::MouseDown(message, wParam);
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        InputManager::MouseUp(message, wParam);

        break;
    }
}

void BaseGame::UpdateInputReset() 
{ 
	InputManager::ResetAllKey();
}


void BaseGame::Update() {


	::TestUpdateFN(this);
	// 씬업데이트
	float dt = m_pTimerManager->GetDeltaTime();

	// 위치 이동 처리
	PhysicsManager::GetI( )->UpdatePhysics(dt);
	// 충돌처리업데이트
	CollisionManager::GetI( )->UpdateCollisions( );

    m_CurrentScene->UpdateLoop( dt );

	

	Test_UpdateRaycast( );



	// Timer
    float fps = m_pTimerManager->GetFPS();
    //WCHAR msg[256];
    //sprintf_s(nullptr, 0, "");
    //swprintf_s(msg, 256, "%.2f FPS", fps);

	std::wstring msg = std::format(L"{:.0f} FPS", fps);

    Vec2 pos(50, 20);
    DebugObjectManager::Instance().DrawDebugText(msg, pos);
	DebugObjectManager::Instance().AllDebugUpdate(dt);
}

void BaseGame::AllUpdate() 
{ 
	UpdateTimer();
    Update();
    // UpdateInputReset 순서 중요 g_BaseGame.Update() 바로 앞에 있으면 눌러지는것이 안될수 있음
    UpdateInputReset();
}

void BaseGame::Render(HDC p_hdc, RECT& p_clientRect)
{
    // 2. 배경 지우기 (하얀색)
    HBRUSH bg = CreateSolidBrush(m_BGColor );
    FillRect(p_hdc, &p_clientRect, bg);
    DeleteObject(bg);

	m_CurrentScene->Render(p_hdc);


	// 콜리젼 그리기
	CollisionManager::GetI()->DrawColliders(p_hdc);

	PhysicsManager::GetI( )->DrawPhysics(p_hdc);

	// 디버그용 자료 그리기
	DebugObjectManager::Instance().AllDebugRender(p_hdc);
}

void BaseGame::UpdateDebugEnd() 
{
	DebugObjectManager::Instance().ResetAllDebugObjects();
}

void BaseGame::SetWindowSize(HWND hwnd) 
{
	m_Hwnd = hwnd;
	CameraManager::GetI()->SetWindowSize(hwnd); // 카메라 매니저에 핸들 설정
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

	//Test_Button();

	//Test_Image();

	//Test_TextLabel();

	////Test_Collider();

	////Test_TimerLoger( );

	Test_RigidBody( );



	// 라인그리기
    Gdiplus::PointF startPoint(50, 50);
    Gdiplus::PointF endPoint(120, 70);
    GameObject* lineobj = m_CurrentScene->CreateObject("line");
    LineComponent* linecom = lineobj->AddComponent<LineComponent>(startPoint, endPoint);
    linecom->SetPoints({ 50, 50 }, {120, 70} );
    linecom->transform->SetWorldPosition(100, 100);

	// 디버깅용 박스그리기
    Gdiplus::RectF rectF(100, 100, 200, 100);
	GameObject* linerect = m_CurrentScene->CreateObject("linerect");
    RectLineComponent* rectlinecom = linerect->AddComponent<RectLineComponent>(rectF);



	// 카메라 세팅
	Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
	mainCamera->SetWorldPosition(0, 0);
    //mainCamera->SetZoom(0.5f); // 카메라 줌 설정
	//mainCamera->SetWorldRotation(-45.f); // 카메라 회전 설정


	


}

void BaseGame::Test_Image() 
{
	return;

    // 이미지 2
    GameObject* pngimgobj2 = m_CurrentScene->CreateObject("PngImageObject2");
    auto* pngcom2 = pngimgobj2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    pngcom2->ImageLoadImage(L"Assets/Images/UVTexture.png");
    pngimgobj2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
    pngimgobj2->transform->SetWorldRotation(-45.f); // 월드 회전 설정
    pngimgobj2->transform->SetWorldPosition(150, 200);
    pngimgobj2->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
    pngcom2->SetISDebugBoundBox(true);


    // 100개 화면에 그리면 10프레임 미만됨 최적화는 나중에
    // 20개 30프레임 정도
    for (size_t i = 0; i < 0; i++) {
        int randx = MyUtil::GetRandInt(100, 900);
        int randy = MyUtil::GetRandInt(50, 500);

        std::string name = std::format("{}objimg", i);
        GameObject* pngimgobj2 = m_CurrentScene->CreateObject(name);
        auto* pngcom2 = pngimgobj2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
        pngcom2->ImageLoadImage(L"Assets/Images/UVTexture.png");
        pngimgobj2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
        pngimgobj2->transform->SetWorldRotation(-45.f); // 월드 회전 설정
        pngimgobj2->transform->SetWorldPosition(randx, randy);
        pngimgobj2->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
        pngcom2->SetISDebugBoundBox(true);
    }


    // 이미지 3
    GameObject* pngimgobj3 = m_CurrentScene->CreateObject("PngImageObject3");
    auto* pngcom3 = pngimgobj3->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    pngimgobj3->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/UVTexture.png");
    pngimgobj3->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
    pngimgobj3->transform->SetWorldRotation(45.f); // 월드 회전 설정
    pngimgobj3->transform->SetWorldPosition(150, 200);
    //pngimgobj3->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
    pngimgobj3->transform->SetParent(pngimgobj2->transform); // 부모 설정
    pngcom3->SetISDebugBoundBox(true);


    // 이미지 1
    GameObject* pngimgobj = m_CurrentScene->CreateObject("PngImageObject");
    //pngimgobj->AddComponent<ImageComponent>();
    pngimgobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    //pngimgobj->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/Mobile - Final Fantasy Record Keeper - Elarra Eiko Model.png");
    pngimgobj->GetComponent<ImageComponent>()->ImageLoadImage(L"Assets/Images/UVTexture.png");
    pngimgobj->transform->SetWorldPosition(200, 200);
    pngimgobj->GetComponent<ImageComponent>()->SetEnabled(false);


    // 이미지 1
    GameObject* centerobj = m_CurrentScene->CreateObject("center");
    g_centerobj = centerobj; // 전역 변수에 저장`
    g_tempimgcomp = centerobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
    g_tempimgcomp->ImageLoadImage(L"Assets/Images/UVTexture.png"); // 525사이즈
    //centerobj->GetComponent<ImageComponent>()->SetDrawRect(0, 0, 64, 64);
    //centerobj->GetComponent<ImageComponent>()->SetSize(128, 128);
    //centerobj->transform->SetWorldPosition(-64, -64);
    g_tempimgcomp->SetISDebugBoundBox(true);


    static Vec2 wpos = { -16, -16 };
    static Vec2 rect = { 64, 64 };
    static Vec2 size = { 32, 32 };
    centerobj->GetComponent<ImageComponent>()->SetDrawRect(0, 0, rect.x, rect.y);
    centerobj->GetComponent<ImageComponent>()->SetSize(size.x, size.y);
    centerobj->transform->SetWorldPosition(0.f, 0.f);
    centerobj->transform->SetPivotPos(wpos.x, wpos.y); // 피봇 위치 설정


}

void BaseGame::Test_Button() 
{
    // 버턴 1
    GameObject* bunobj = m_CurrentScene->CreateObject("TestObject");
    //bunobj->AddComponent< Button>();
    bunobj->AddComponent<Button>([](Button* p_owerbtn) {
        // 버튼 클릭시 동작
        MessageBox(nullptr, L"Button Clicked!", L"Info", MB_OK);
    });
    bunobj->GetComponent<Transform>()->setLocalPosition(50, 100);
    bunobj->GetComponent<Transform>()->SetWorldRotation(45.f); // 월드 회전 설정


    // 버턴 2
    GameObject* bunobj2 = m_CurrentScene->CreateObject("TestObject2");
    bunobj2->AddComponent<Button>([](Button* p_owerbtn) {
        // 버튼 클릭시 동작
        MessageBox(nullptr, L"버턴2!", L"Info", MB_OK);
    });
    bunobj2->GetComponent<Transform>()->setLocalPosition(150, -20);
    bunobj2->GetComponent<Transform>()->SetParent(bunobj->GetComponent<Transform>()); // 부모 설정

}

void BaseGame::Test_TextLabel() 
{

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


}

void BaseGame::Test_Collider() 
{ 
	CollisionManager::GetI( )->SetIsDebugDraw(true);


	// 충돌처리 오브젝트1
    GameObject* colliderbox1 = m_CurrentScene->CreateObject("ColliderBox1");
    auto* pngcom1 = colliderbox1->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	pngcom1->ImageLoadImage(L"Assets/Images/UVTexture.png");
    colliderbox1->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
    colliderbox1->transform->SetWorldRotation(0.f); // 월드 회전 설정
    colliderbox1->transform->SetWorldPosition(400, 200);
    colliderbox1->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
    //pngcom1->SetISDebugBoundBox(true);

    Vec2 pos1(400, 200);
    SizeF size1(100, 100);
    BoxCollider* boxcol1 = colliderbox1->AddComponent<BoxCollider>(pos1, 0, size1, 100, 100);
	boxcol1->SetBoxCollider(pos1, 0, size1, size1.Width, size1.Height);



	// 충돌처리 오브젝트2
	GameObject* colliderbox2 = m_CurrentScene->CreateObject("ColliderBox2");
	auto* pngcom2 = colliderbox2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	pngcom2->ImageLoadImage(L"Assets/Images/UVTexture.png");
	colliderbox2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
	colliderbox2->transform->SetWorldRotation(0.f); // 월드 회전 설정
	colliderbox2->transform->SetWorldPosition(430, 200);
	colliderbox2->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
	//pngcom2->SetISDebugBoundBox(true);

	Vec2 pos2(430, 200);
	SizeF size2(100, 100);
	BoxCollider* boxcol2 = colliderbox2->AddComponent<BoxCollider>(pos2, 0, size2, 100, 100);
	boxcol2->SetBoxCollider(pos2, 0, size2, size1.Width, size1.Height);
	



	// 바인딩 방식 호출
	UtilTimerManager::GetI( )->AddTimer(3.f
		, nullptr //std::bind(&BaseGame::Test_AniCallBackFN, this, std::placeholders::_1)
		, std::bind(&BaseGame::Test_AniCallBackFN1, this, std::placeholders::_1, nullptr )
		, nullptr
		, 0
	);

	// 람다 바인딩으로 보내는 방식
	UtilTimerManager::GetI( )->AddTimer(4.f
		, [boxcol1, boxcol2] (UtilTimer* utiltimer, void* p_sender)
		{
			static float moveval = 0.1f;
			Vec2& wpos = boxcol2->transform->GetWorldPosition( );
			wpos.x += moveval;

			if ( wpos.x > 520.f )
				wpos.x = 430.f;

			boxcol2->transform->SetWorldPosition(wpos);
		}
		, std::bind(&BaseGame::Test_AniCallBackFN, this, std::placeholders::_1, nullptr)
		, nullptr
		, -1
	);


}


void BaseGame::Test_TimerLoger( )
{
	UtilTimerManager::GetI( )->AddTimer(2.0f
		, [ ] (UtilTimer* utiltimer, void* p_sender) {
			//std::wcout << L"[Timer1] 매 프레임 업로드!\n"; 
			UtilLoger::Log(L"[Timer1] 매 프레임 업로드!", E_LogLevel::E_WARN);
		}
		, [ ] (UtilTimer* utiltimer, void* p_sender) {
			//std::wcout << L"[Timer1] 2초 후 실행됨!\n"; 
			UtilLoger::Log(L"[Timer1] 2초 후 실행됨!", E_LogLevel::E_ERROR);
		}
		, 0
	);


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

void BaseGame::Test_UpdateRaycast( )
{
	PointF stpos(10, 190);
	PointF endpos(600, 190);
	PointF hitpos;
	auto* collider = CollisionManager::GetI( )->Raycast(stpos, endpos, hitpos);


	Gdiplus::Color col(255, 255, 0, 0);
	if ( collider )
	{
		col.SetValue(Color::Blue);
	}

	Vec2 vstpos(stpos.X, stpos.Y);
	Vec2 vendpos(endpos.X, endpos.Y);
	DebugObjectManager::Instance( ).DrawLine(vstpos, vendpos, 1.f, col);


}

void BaseGame::Test_RigidBody( )
{

	CollisionManager::GetI( )->SetIsDebugDraw(true);


	// 충돌처리 오브젝트1
	GameObject* rd_gameobject = m_CurrentScene->CreateObject("RDBox01");
	//auto* pngcom1 = rd_gameobject->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	//pngcom1->ImageLoadImage(L"Assets/Images/UVTexture.png");
	rd_gameobject->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
	rd_gameobject->transform->SetWorldRotation(0.f); // 월드 회전 설정
	rd_gameobject->transform->SetWorldPosition(400, 200);
	rd_gameobject->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
	//pngcom1->SetISDebugBoundBox(true);

	Vec2 pos1(451, 200);
	SizeF size1(100, 100);
	BoxCollider* boxcol1 = rd_gameobject->AddComponent<BoxCollider>(pos1, 0, size1, 100, 100);
	boxcol1->SetBoxCollider(pos1, 0, size1, size1.Width, size1.Height);
	RigidbodyComponent* rigidcom = rd_gameobject->AddComponent<RigidbodyComponent>();

	// 플레이어 연결
	//m_playerbody = rd_gameobject;
	m_TestPlayerMoveCom = rd_gameobject->AddComponent< Test_PlayerMoveCom>( );





	GameObject* colliderbox2 = m_CurrentScene->CreateObject("RDBox02");
	//auto* pngcom2 = colliderbox2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	//pngcom2->ImageLoadImage(L"Assets/Images/UVTexture.png");
	colliderbox2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
	colliderbox2->transform->SetWorldRotation(0.f); // 월드 회전 설정
	colliderbox2->transform->SetWorldPosition(400, 200);
	colliderbox2->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정
	//pngcom1->SetISDebugBoundBox(true);

	Vec2 pos2(400, 300);
	SizeF size2(100, 100);
	BoxCollider* boxcol2 = colliderbox2->AddComponent<BoxCollider>(pos2, 0, size2, 100, 100);
	boxcol2->SetBoxCollider(pos2, 0, size2, size2.Width, size2.Height);



	Test_RigidBoxCollider(500, 350, "RDBox03");

	Test_RigidBoxCollider(600, 400, "RDBox04");
}

void BaseGame::Test_RigidBoxCollider(float p_x, float p_y, std::string p_objname)
{

	GameObject* colliderbox2 = m_CurrentScene->CreateObject(p_objname);
	//auto* pngcom2 = colliderbox2->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	//pngcom2->ImageLoadImage(L"Assets/Images/UVTexture.png");
	colliderbox2->transform->SetPivotPos(-100, -100); // 피봇 위치 설정
	colliderbox2->transform->SetWorldRotation(0.f); // 월드 회전 설정
	colliderbox2->transform->SetWorldPosition(400, 200);
	colliderbox2->transform->SetWorldScale(0.5f, 0.5f); // 월드 스케일 설정

	Vec2 pos2(p_x, p_y);
	SizeF size2(100, 100);
	BoxCollider* boxcol2 = colliderbox2->AddComponent<BoxCollider>(pos2, 0, size2, 100, 100);
	boxcol2->SetBoxCollider(pos2, 0, size2, size2.Width, size2.Height);

}

