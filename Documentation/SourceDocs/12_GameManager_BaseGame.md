# 12. 게임 매니저와 생명주기 (Game Manager & Lifecycle)

## 소개 (Introduction)

`BaseGame` 클래스는 이 프레임워크의 심장부입니다. 윈도우 생성 후, 게임의 모든 시스템(Scene, Input, Timer, Physics 등)을 초기화하고 매 프레임마다 업데이트 및 렌더링을 지시하는 중앙 관리자 역할을 합니다.

**파일 경로:** `FrameWork_2DAPI/BaseGame.h`, `FrameWork_2DAPI/BaseGame.cpp`

## 핵심 개념 (Core Concepts)

- **중앙 집중식 관리:** 게임의 핵심 시스템(시간, 입력, 씬 등)을 하나의 클래스(`BaseGame`)에서 관리하여 흐름을 파악하기 쉽게 만듭니다.
- **게임 루프 단계:**
    - **초기화 (Init):** GDI+, 콘솔, 매니저, 초기 씬 등을 설정합니다.
    - **입력 처리 (Input):** 사용자 입력을 받아 처리합니다.
    - **업데이트 (Update):** 게임 로직, 물리, 충돌, 씬 업데이트를 수행합니다.
    - **렌더링 (Render):** 변경된 상태를 화면에 그립니다.
    - **종료 (Release):** 사용한 메모리와 리소스를 해제합니다.
- **GDI+ 사용:** Windows의 기본 그래픽 API인 GDI+를 초기화하고 사용하여 이미지를 그립니다.

## 주요 기능 및 메서드 (Key Features & Methods)

### `Init(HWND p_hwnd)`

게임 실행 시 `wWinMain`에서 가장 먼저 호출됩니다.

1.  **GDI+ 초기화:** `InitGDIPlus()`를 호출하여 이미지 그리기 기능을 켭니다.
2.  **매니저 초기화:** `FacadeManager`, `TimerManager`, `InputManager` 등 핵심 도구들을 생성합니다.
3.  **초기 씬 설정:** `Test_InitScene()` 등을 통해 게임 시작 시 보여줄 화면을 구성합니다.

```cpp
void BaseGame::Init(HWND p_hwnd)
{
    InitConsoleWindow();   // 디버깅용 콘솔창 열기
    InitGDIPlus();         // 그래픽 엔진 시작

    m_Hwnd = p_hwnd;
    m_pTimerManager = new TimerManager(); // 시간 관리자 생성
    m_pInputManager = new InputManager(); // 입력 관리자 생성

    // ... 씬 초기화 ...
}
```

### `AllUpdate()`

매 프레임 호출되는 메인 업데이트 함수입니다.

1.  **시간 업데이트:** `UpdateTimer()`를 통해 프레임 간의 시간 차이(Delta Time)를 계산합니다.
2.  **게임 로직:** `Update()`를 호출하여 물리, 충돌, 씬 로직을 처리합니다.
3.  **입력 리셋:** `UpdateInputReset()`으로 이번 프레임에 처리된 키 입력 상태를 정리합니다.

### `Update()`

실질적인 게임 로직이 돌아가는 곳입니다.

```cpp
void BaseGame::Update() {
    float dt = m_pTimerManager->GetDeltaTime();

    // 1. 물리 시뮬레이션 (이동 처리)
    PhysicsManager::GetI()->UpdatePhysics(dt);

    // 2. 충돌 감지 및 처리
    CollisionManager::GetI()->UpdateCollisions();

    // 3. 현재 씬(Scene)의 모든 객체 업데이트
    m_CurrentScene->UpdateLoop(dt);

    // ... 디버그 정보 표시 ...
}
```

### `Render(HDC p_hdc, RECT& p_clientRect)`

화면에 모든 것을 그리는 함수입니다. 더블 버퍼링된 `memDC`가 인자로 넘어옵니다.

1.  **배경 지우기:** `FillRect`로 화면을 깨끗하게 지웁니다.
2.  **씬 렌더링:** `m_CurrentScene->Render(p_hdc)`를 호출하여 현재 씬의 모든 `GameObject`를 그립니다.
3.  **디버그 렌더링:** 충돌 박스, 물리 정보 등을 개발자용으로 덧그립니다.
4.  **리소스 정리 (중요):** `Prev_DestroyQueueObjects...` 등을 호출하여 삭제 대기 중인 오브젝트들을 메모리에서 실제로 해제합니다. (지연된 삭제)

```cpp
void BaseGame::Render(HDC p_hdc, RECT& p_clientRect)
{
    // ... 배경 지우기 ...

    m_CurrentScene->Render(p_hdc); // 게임 객체 그리기

    // ... 디버그 그리기 (충돌 박스 등) ...
    CollisionManager::GetI()->DrawColliders(p_hdc);

    // 삭제 대기 중인 오브젝트들 메모리 해제 (Render 끝난 후 안전하게)
    m_CurrentScene->ProcessDestroyQueue();
}
```

## 사용법 (Usage)

학생들이 `BaseGame` 클래스 자체를 수정할 일은 많지 않지만, 게임의 **시작점**을 바꾸거나 **전역적인 테스트**를 할 때 자주 보게 됩니다.

### 초기 테스트 코드 작성 (`Test_InitScene`)

`BaseGame.cpp` 하단의 `Test_InitScene` 함수는 게임이 켜지자마자 실행될 코드를 담고 있습니다. 여기에 여러분의 테스트 코드를 작성할 수 있습니다.

```cpp
void BaseGame::Test_InitScene()
{
    // 예: 내 캐릭터 생성 테스트
    GameObject* myObj = m_CurrentScene->CreateObject("MyHero");
    myObj->AddComponent<ImageComponent>(...);
}
```

## 소스 코드 분석 (Source Code Analysis)

- **지연된 삭제 (Delayed Destruction):**
    - `Update()` 중에 오브젝트를 `delete` 해버리면, 씬 리스트를 순회하던 루프가 깨질 수 있습니다.
    - 따라서 `Destroy()` 요청이 오면 "삭제 대기열"에만 넣어두고, 모든 로직과 렌더링이 끝난 `Render()` 함수의 마지막 부분에서 `ProcessDestroyQueue()`를 통해 안전하게 삭제합니다.
- **싱글톤 패턴 활용:** `TimerManager`, `InputManager` 등은 `BaseGame`이 멤버 변수로 가지고 있거나 `FacadeManager` 등을 통해 전역적으로 접근 가능한 형태로 관리됩니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`Update` 함수와 `Render` 함수를 분리해야 하는 이유는 무엇인가요? 만약 `Render` 안에서 게임 로직(위치 이동 등)을 처리하면 어떤 문제가 생길까요?"
- "왜 오브젝트를 즉시 삭제(`delete`)하지 않고, `ProcessDestroyQueue`를 통해 나중에 한꺼번에 삭제하나요?"
- "`BaseGame` 클래스에서 `GdiplusStartup`을 호출하는 이유는 무엇이며, 이를 하지 않으면 어떻게 되나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`BaseGame::Init` 함수에서 내가 만든 새로운 `Scene`(예: `TitleScene`)으로 시작하도록 코드를 변경해 줘."
- "게임의 배경색을 파란색으로 변경하고 싶어. `BaseGame::Render` 함수에서 배경을 지우는 코드를 어떻게 수정해야 할까?"
- "`BaseGame`에 `Pause` 기능을 추가하고 싶어. `bool isPaused` 변수를 만들고, P 키를 누르면 `Update` 로직이 멈추도록 구현해 줘."
