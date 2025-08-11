#include "Base_FrameWorkCls.h"

#include <Windows.h>
#include <windows.h>
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

Base_FrameWorkCls::Base_FrameWorkCls( ) : gdiplusToken(0)
{
	m_CurrentScene = new Scene( );
}

Base_FrameWorkCls::~Base_FrameWorkCls( )
{
	Release( );
}

int Base_FrameWorkCls::InGameMessageLoopWile( )
{

	MSG msg;


	//// 기본 메시지 루프입니다:
	//while ( GetMessage(&msg, nullptr, 0, 0) )
	//{
	//	if ( !TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg) )
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	while ( m_isRunning ) {

		//      g_BaseGame.UpdateTimer();
		//      g_BaseGame.Update();
			  //// UpdateInputReset 순서 중요 g_BaseGame.Update() 바로 앞에 있으면 눌러지는것이 안될수 있음
		//      g_BaseGame.UpdateInputReset();
		this->AllUpdate( );

		while ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) ) {
			if ( msg.message == WM_QUIT ) {
				m_isRunning = false;
				break;
			}

			if ( !TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg) ) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		// g_hWnd 사용해도됨
		// 깜빡임 줄이기 위해 TRUE → FALSE
		InvalidateRect(msg.hwnd, nullptr, FALSE);

		//// 게임 로직 / 렌더링
		//Update();   // 예: 게임 상태 갱신
		//Render();   // 예: 그리기
		////Sleep(16);  // or 프레임 타이머에 따른 deltaTime 조절

		//g_BaseGame.Run();


		this->UpdateDebugEnd( );

	}


	return ( int ) msg.wParam;
}

LRESULT Base_FrameWorkCls::InGameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	this->UpdateInput(message, wParam, lParam);

	switch ( message )
	{
	case WM_COMMAND:
	{
		//int wmId = LOWORD(wParam);
		//// 메뉴 선택을 구문 분석합니다:
		//switch ( wmId )
		//{
		//case IDM_ABOUT:
		//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//	break;
		//case IDM_EXIT:
		//	DestroyWindow(hWnd);
		//	break;
		//default:
		//	return DefWindowProc(hWnd, message, wParam, lParam);
		//}
	}
	break;
	case WM_SIZING:
	{

		this->SetWindowSize(hWnd); // 윈도우 크기 조정 시 g_BaseGame에 핸들 설정
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...


		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		int width = clientRect.right;
		int height = clientRect.bottom;

		// 1. 백 버퍼용 메모리 DC 및 비트맵 생성
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP backBuffer = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP oldBitmap = ( HBITMAP ) SelectObject(memDC, backBuffer);

		//// 2. 배경 지우기 (하얀색)
		//HBRUSH bg = CreateSolidBrush(RGB(255, 255, 255));
		//FillRect(memDC, &clientRect, bg);
		//DeleteObject(bg);
		//Test_DrawRectBox(memDC, clientRect, width);

		this->Render(memDC, clientRect);

		// 4. 최종적으로 메모리 DC → 화면 DC 전송
		BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

		// 5. 리소스 정리
		SelectObject(memDC, oldBitmap);
		DeleteObject(backBuffer);
		DeleteDC(memDC);


		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Base_FrameWorkCls::Init(HWND p_hwnd, HACCEL p_haccel, HINSTANCE p_hinstance)
{
	InitConsoleWindow( );

	InitGDIPlus( );

	m_Hwnd = p_hwnd;
	m_hAccelTable = p_haccel;
	m_hInstance = p_hinstance;

	FacadeManager::GetI( )->Initlize( ); // Initialize FacadeManager
	CameraManager::GetI( )->SetWindowSize(p_hwnd); // Set the window handle for CameraManager

	m_pTimerManager = new TimerManager( );
	m_pInputManager = new InputManager( );

	InitSettings( );
}

void Base_FrameWorkCls::Release( )
{
	if ( m_CurrentScene )
	{
		m_CurrentScene->Release( );
		delete m_CurrentScene;
		m_CurrentScene = nullptr;
	}

	if ( m_pTimerManager )
	{
		delete m_pTimerManager;
		m_pTimerManager = nullptr;
	}

	if ( m_pInputManager )
	{
		delete m_pInputManager;
		m_pInputManager = nullptr;
	}


	FacadeManager::GetI( )->DestroyManager( );

	ReleaseGDIPlus( );
}

void Base_FrameWorkCls::Run( )
{
	m_CurrentScene->Start( );

	MSG msg = {};
	while ( isRunning ) {
		if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			if ( msg.message == WM_QUIT ) isRunning = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//currentScene.Update();

		// 렌더링 호출 등
	}
}

void Base_FrameWorkCls::CleanUp( )
{
}

void Base_FrameWorkCls::UpdateInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 타이머및 인풋쪽 값들
	//m_pInputManager->MouseMove(lParam);

	switch ( message ) {
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

void Base_FrameWorkCls::AllUpdate( )
{
	UpdateTimer( );
	Update( );
	// UpdateInputReset 순서 중요 g_BaseGame.Update() 바로 앞에 있으면 눌러지는것이 안될수 있음
	UpdateInputReset( );

}

void Base_FrameWorkCls::Render(HDC p_hdc, RECT& p_clientRect)
{
	// 2. 배경 지우기 (하얀색)
	HBRUSH bg = CreateSolidBrush(m_BGColor);
	FillRect(p_hdc, &p_clientRect, bg);
	DeleteObject(bg);

	m_CurrentScene->Render(p_hdc);


	// 콜리젼 그리기
	CollisionManager::GetI( )->DrawColliders(p_hdc);

	PhysicsManager::GetI( )->DrawPhysics(p_hdc);

	// 디버그용 자료 그리기
	DebugObjectManager::Instance( ).AllDebugRender(p_hdc);
}

void Base_FrameWorkCls::UpdateDebugEnd( )
{
	DebugObjectManager::Instance( ).ResetAllDebugObjects( );
}

void Base_FrameWorkCls::SetWindowSize(HWND hwnd)
{
	m_Hwnd = hwnd;
	CameraManager::GetI( )->SetWindowSize(hwnd); // 카메라 매니저에 핸들 설정
}

void Base_FrameWorkCls::UpdateInputReset( )
{
	InputManager::ResetAllKey( );
}

void Base_FrameWorkCls::UpdateTimer( )
{
	// 타이머 업데이트 적용

	m_pTimerManager->Update( );

	UtilTimerManager::GetI( )->Update(m_pTimerManager->GetDeltaTime( ));

}

void Base_FrameWorkCls::Update( )
{

	// 씬업데이트
	float dt = m_pTimerManager->GetDeltaTime( );

	// 위치 이동 처리
	PhysicsManager::GetI( )->UpdatePhysics(dt);
	// 충돌처리업데이트
	CollisionManager::GetI( )->UpdateCollisions( );

	m_CurrentScene->Update(dt);




	// Timer
	float fps = m_pTimerManager->GetFPS( );
	//WCHAR msg[256];
	//sprintf_s(nullptr, 0, "");
	//swprintf_s(msg, 256, "%.2f FPS", fps);

	std::wstring msg = std::format(L"{:.0f} FPS", fps);

	Vec2 pos(50, 20);
	DebugObjectManager::Instance( ).DrawDebugText(msg, pos);
	DebugObjectManager::Instance( ).AllDebugUpdate(dt);

}

void Base_FrameWorkCls::InitConsoleWindow( )
{
	AllocConsole( ); // 콘솔창 생성
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
	std::wcout.clear( );

	std::wcout << L"[Console Initialized] 초기화\n" << std::endl;
	std::wcout.flush( );
}

void Base_FrameWorkCls::InitGDIPlus( )
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void Base_FrameWorkCls::ReleaseGDIPlus( )
{
	GdiplusShutdown(gdiplusToken);
}
