// FrameWork_2DAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "FrameWork_2DAPI.h"

#include "BaseGame.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Test_DrawRectBox(HDC memDC, RECT& clientRect, int width);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


/// <summary>
/// 
/// </summary>

HWND g_hWnd = nullptr; // 전역 윈도우 핸들
BaseGame g_BaseGame;
bool g_isRunning = true; // 게임 루프를 제어하는 변수




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAMEWORK2DAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAMEWORK2DAPI));

    MSG msg;

    //// 기본 메시지 루프입니다:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}


    g_BaseGame.Init(g_hWnd);
    while (g_isRunning) {
        g_BaseGame.UpdateTimer();
        g_BaseGame.Update();

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_isRunning = false;
                break;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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

		g_BaseGame.UpdateDebugEnd();
    }


    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRAMEWORK2DAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FRAMEWORK2DAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

int x = 0; // 사각형의 x 위치를 저장하는 전역 변수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    g_BaseGame.UpdateInput(message, wParam, lParam);

    switch (message)
    {
    case WM_TIMER: 
		{

		} break;
    case WM_SIZING: 
		{
		
			g_BaseGame.SetWindowSize(hWnd); // 윈도우 크기 조정 시 g_BaseGame에 핸들 설정
		}
        break;
    case WM_SIZE:
		{

		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, backBuffer);

            //// 2. 배경 지우기 (하얀색)
            //HBRUSH bg = CreateSolidBrush(RGB(255, 255, 255));
            //FillRect(memDC, &clientRect, bg);
            //DeleteObject(bg);
            //Test_DrawRectBox(memDC, clientRect, width);

			g_BaseGame.Render(memDC, clientRect);

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


void Test_DrawRectBox(HDC memDC, RECT& clientRect, int width)
{
    // 3. 그리기 (예: 움직이는 사각형)
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(memDC, pen);
    SelectObject(memDC, brush);

    //x = 100;
    Rectangle(memDC, 100 + x, 100, 200 + x, 200);
    x = (x + 2) % width;  // 위치 이동

    DeleteObject(brush);
    DeleteObject(pen);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
