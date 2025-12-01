# 11. 윈도우 생성 및 메인 루프 (Window Creation & Main Loop)

## 소개 (Introduction)
이 문서는 Windows API를 사용하여 게임 윈도우를 생성하고, 게임의 핵심인 '메인 루프(Main Loop)'를 설정하는 방법을 설명합니다. `FrameWork_2DAPI.cpp` 파일은 애플리케이션의 진입점(Entry Point)이며, 여기서 프로그램이 시작되고 종료될 때까지의 전체적인 흐름을 관리합니다.

**파일 경로:** `FrameWork_2DAPI/FrameWork_2DAPI.cpp`

## 핵심 개념 (Core Concepts)

- **진입점 (Entry Point):** C++ 콘솔 프로그램의 `main()`과 달리, Windows GUI 프로그램은 `wWinMain()` 함수에서 시작합니다.
- **메시지 루프 (Message Loop):** 윈도우 운영체제와 애플리케이션 간의 통신 방식입니다. 마우스 클릭, 키보드 입력 등의 이벤트가 메시지 큐에 쌓이면 이를 하나씩 꺼내 처리합니다.
- **게임 루프 (Game Loop):** 일반적인 응용 프로그램과 달리 게임은 사용자의 입력이 없어도 끊임없이 화면을 갱신해야 합니다. 따라서 `PeekMessage`를 활용하여 메시지가 없을 때도 게임 로직(`Update`)과 렌더링(`Render`)을 수행합니다.
- **더블 버퍼링 (Double Buffering):** 화면 깜빡임을 방지하기 위해 보이지 않는 메모리 공간(Back Buffer)에 먼저 그림을 그리고, 완성된 그림을 한 번에 화면으로 복사하는 기법입니다.

## 주요 기능 및 메서드 (Key Features & Methods)

### `wWinMain()`

- **목적:** 프로그램의 시작점입니다. 윈도우 클래스를 등록하고, 윈도우를 생성하며, 게임 루프를 실행합니다.
- **작동 방식:**
    1. `MyRegisterClass()`로 윈도우 스타일 등을 설정하고 등록합니다.
    2. `InitInstance()`로 실제 윈도우를 생성하고 화면에 띄웁니다.
    3. `g_BaseGame.Init()`으로 게임 프레임워크를 초기화합니다.
    4. `while(g_isRunning)` 루프 안에서 메시지 처리와 게임 업데이트(`AllUpdate`)를 반복합니다.

```cpp
// FrameWork_2DAPI.cpp (일부)
int APIENTRY wWinMain(...)
{
    // ... (초기화 코드) ...
    g_BaseGame.Init(g_hWnd); // 게임 초기화

    while (g_isRunning) { // 메인 게임 루프
        g_BaseGame.AllUpdate(); // 게임 로직 업데이트

        // 메시지 처리 (PeekMessage 사용)
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_isRunning = false;
                break;
            }
            // ... (메시지 변환 및 송출) ...
        }

        // 화면 갱신 요청 (WM_PAINT 메시지 유발)
        InvalidateRect(msg.hwnd, nullptr, FALSE);
        g_BaseGame.UpdateDebugEnd();
    }
    return (int) msg.wParam;
}
```

### `WndProc()`

- **목적:** 윈도우 메시지 처리기(Window Procedure)입니다. OS에서 보내온 메시지(창 크기 변경, 그리기 요청, 종료 등)를 처리합니다.
- **작동 방식:** `switch-case` 문을 사용하여 메시지 종류(`WM_PAINT`, `WM_DESTROY` 등)에 따라 적절한 코드를 실행합니다.

### `WM_PAINT`와 더블 버퍼링

- **목적:** 화면을 그릴 때 발생하는 깜빡임(Flickering) 현상을 제거합니다.
- **작동 방식:**
    1. `CreateCompatibleDC`와 `CreateCompatibleBitmap`으로 가상 화면(메모리 DC)을 만듭니다.
    2. 모든 그림(게임 장면)을 메모리 DC에 그립니다 (`g_BaseGame.Render(memDC, ...)`).
    3. `BitBlt` 함수를 사용하여 메모리 DC의 내용을 실제 화면 DC로 고속 복사합니다.

```cpp
// FrameWork_2DAPI.cpp 의 WndProc 내부
case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // 1. 더블 버퍼링을 위한 메모리 DC 생성
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP backBuffer = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, backBuffer);

    // 2. 메모리 DC에 게임 장면 렌더링
    g_BaseGame.Render(memDC, clientRect);

    // 3. 메모리 DC의 내용을 실제 화면으로 복사 (BitBlt)
    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    // 4. 리소스 정리
    SelectObject(memDC, oldBitmap);
    DeleteObject(backBuffer);
    DeleteDC(memDC);

    EndPaint(hWnd, &ps);
}
break;
```

## 사용법 (Usage)

이 파일은 프레임워크의 엔진 부분에 해당하므로, 학생들이 직접 수정할 일은 많지 않습니다. 하지만 게임의 해상도를 변경하거나, 종료 조건을 변경하고 싶을 때 참고할 수 있습니다.

### 게임 윈도우 크기 변경 시

`WM_SIZING` 메시지에서 `g_BaseGame.SetWindowSize(hWnd)`가 호출되어 게임 내 해상도 정보가 갱신됩니다.

```cpp
case WM_SIZING:
{
    g_BaseGame.SetWindowSize(hWnd); // 윈도우 크기 조정 시 처리
}
break;
```

## 소스 코드 분석 (Source Code Analysis)

- **`PeekMessage` vs `GetMessage`:**
    - `GetMessage`: 메시지 큐가 비어있으면 메시지가 올 때까지 대기(Block)합니다. 일반적인 사무용 프로그램에 적합합니다.
    - `PeekMessage`: 메시지 큐가 비어있어도 즉시 반환(Return)합니다. 따라서 메시지가 없는 동안에도 게임 로직을 실행(`g_BaseGame.AllUpdate()`)할 수 있어 게임 루프에 필수적입니다.
- **`InvalidateRect(..., FALSE)`:**
    - `WM_PAINT` 메시지를 강제로 발생시켜 화면을 다시 그리게 합니다.
    - 마지막 인자를 `FALSE`로 설정하여 배경을 지우지 않도록 함으로써 깜빡임을 줄입니다 (배경은 `BitBlt`로 덮어쓰기 때문).

## 참고 자료 (References)

- [MSDN: WinMain entry point](https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point)
- [MSDN: Window Messages](https://docs.microsoft.com/en-us/windows/win32/learnwin32/window-messages)
- [Game Loop Pattern](https://gameprogrammingpatterns.com/game-loop.html)

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`PeekMessage`와 `GetMessage`의 차이점은 무엇이며, 왜 게임에서는 `PeekMessage`를 사용해야 하나요?"
- "더블 버퍼링(Double Buffering)을 사용하지 않고 화면에 직접 그리면(`hDC`에 바로 그리기) 어떤 시각적 문제가 발생하나요?"
- "`wWinMain` 함수에서 `g_BaseGame.Init()`을 호출하지 않으면 어떤 일이 발생할까요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "윈도우의 배경색을 흰색 대신 검은색이나 다른 색으로 초기화하려면 `WndProc`의 `WM_PAINT` 처리 부분을 어떻게 수정해야 하나요?"
- "게임이 실행되는 동안 ESC 키를 눌렀을 때 게임을 종료(`g_isRunning = false`)시키는 코드를 `WndProc`나 `UpdateInput` 어디에 추가해야 하나요?"
- "현재 프레임워크는 고정 프레임인가요 가변 프레임인가요? 프레임 속도를 제한(예: 60FPS)하려면 메인 루프에 어떤 코드를 추가해야 할까요?"
