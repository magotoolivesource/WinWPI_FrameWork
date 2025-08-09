#include "InGame_PVSZ.h"
// Windows 헤더 파일
#include <windows.h>

#include "PVSZ.h"



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
	// 기본 게임 구현적용
	m_CurrentScene->PrevInitSettings( );
	m_CurrentScene->InitSettings( );
}
