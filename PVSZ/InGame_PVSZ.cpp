#include "InGame_PVSZ.h"
// Windows 헤더 파일
#include <windows.h>

#include "PVSZ.h"

#include "Stage01.h"



INT_PTR CALLBACK    PVSZAbout(HWND, UINT, WPARAM, LPARAM);


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK PVSZAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch ( message )
	{
	case WM_INITDIALOG:
		return ( INT_PTR ) TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL )
		{
			EndDialog(hDlg, LOWORD(wParam));
			return ( INT_PTR ) TRUE;
		}
		break;
	}
	return ( INT_PTR ) FALSE;
}


Scene* InGame_PVSZ::GetCurrentScene( )
{
	return m_CurrentScene;
}

GameObject* InGame_PVSZ::AddCurrentSceneGameObject(const std::string& name)
{
	if ( !m_CurrentScene )
	{
		std::string msg = "오브젝트 생성 에러 루트 씬이 없음 확인하기 : " + name;
		throw std::runtime_error(msg);
		return nullptr;
	}

	return m_CurrentScene->CreateObject(name);
}

GameObject* InGame_PVSZ::AddGameObject(const std::string& name)
{
	return InGame_PVSZ::GetI( )->AddCurrentSceneGameObject(name);
}

InGame_PVSZ::InGame_PVSZ( )
{
}

InGame_PVSZ::~InGame_PVSZ( )
{
}

LRESULT InGame_PVSZ::InGameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	LRESULT result = __super::InGameWndProc(hWnd, message, wParam, lParam);

	switch ( message )
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch ( wmId )
		{
		case IDM_ABOUT:
			DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, PVSZAbout);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	}



	return result;
}

void InGame_PVSZ::InitSettings( )
{
	m_AllSceneVec.resize(( int ) E_STAGETYPE::MAX);
	for ( auto item : m_AllSceneVec )
	{
		item = nullptr;
	}


	InitStage01( );

	

	SetStageChangeType(E_STAGETYPE::STAGE01);
}

void InGame_PVSZ::InitStage01( )
{
	Stage01* stage1 = new Stage01( );
	//m_AllSceneVec.push_back(stage1);
	m_AllSceneVec[ ( int ) E_STAGETYPE::STAGE01 ] = stage1;
	
	
}

void InGame_PVSZ::SetStageChangeType(E_STAGETYPE p_stagetype)
{
	if ( m_CurrentScene )
	{
		m_CurrentScene->Release( );
		m_CurrentScene = nullptr;
	}

	m_SelectStageType = p_stagetype;

	m_CurrentScene = m_AllSceneVec[ ( int ) m_SelectStageType ];

	if ( m_CurrentScene )
	{
		// 기본 게임 구현적용
		m_CurrentScene->PrevInitSettings( );
		m_CurrentScene->InitSettings( );
	}

}
