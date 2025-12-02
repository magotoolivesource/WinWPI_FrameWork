# 15. 핵심 매니저: 입력과 시간 (Core Managers: Input & Time)

## 소개 (Introduction)

게임을 구성하는 가장 기본적인 두 가지 요소는 "사용자의 입력을 받는 것"과 "시간의 흐름을 측정하는 것"입니다.
`InputManager`는 키보드와 마우스의 상태를 관리하고, `TimerManager`는 프레임 간의 시간 간격(Delta Time)을 계산하여 게임의 속도를 일정하게 유지합니다.

**관련 파일:**
- `FrameWork_2DAPI/Core/InputManager.h`, `InputManager.cpp`
- `FrameWork_2DAPI/Core/TimerManager.h`

## 1. InputManager (입력 관리자)

Windows 메시지(`WM_KEYDOWN`, `WM_MOUSEMOVE` 등)를 받아 게임 로직에서 사용하기 쉬운 형태로 변환해줍니다.

### 핵심 개념

- **상태 구분:**
    - **Down:** 키를 **누르는 순간** (한 번만 참). 공격, 점프 등에 사용.
    - **Pressed:** 키를 **누르고 있는 동안** (계속 참). 이동 등에 사용.
    - **Up:** 키를 **떼는 순간** (한 번만 참). 차징 공격 발사 등에 사용.
- **정적 클래스 (Static Class):** 모든 메서드와 변수가 `static`으로 선언되어 있어, 어디서든 `InputManager::IsKeyPressed(...)` 형태로 쉽게 접근할 수 있습니다.

### 주요 메서드

#### `Init()` & `ResetAllKey()`
입력 상태를 초기화합니다. 특히 `ResetAllKey()`는 매 프레임 끝(`BaseGame::AllUpdate` 내부)에서 호출되어 `Down`과 `Up` 상태를 리셋합니다.

#### 키보드 입력 확인
`InputKey` 열거형(Enum)을 사용하여 직관적으로 키를 지정할 수 있습니다.

```cpp
if (InputManager::ISKeyDown(InputKey::Space)) {
    // 점프! (누른 순간 한 번만 실행)
}

if (InputManager::IsKeyPressed(InputKey::RightArrow)) {
    // 오른쪽으로 이동 (누르고 있는 동안 계속 실행)
}
```

#### 마우스 입력 확인

```cpp
if (InputManager::ISMouseDown(E_MOUSEETYPE::LEFT)) {
    // 마우스 왼쪽 클릭 순간
}

POINT mousePos = InputManager::GetMousePosition();
// mousePos.x, mousePos.y 로 마우스 좌표 접근
```

## 2. TimerManager (시간 관리자)

컴퓨터마다 성능이 다르기 때문에, 프레임 수(FPS)도 제각각입니다.
게임이 어떤 컴퓨터에서도 같은 속도로 실행되게 하려면 **델타 타임(Delta Time)**을 사용해야 합니다.

### 핵심 개념

- **고해상도 타이머 (High-Resolution Timer):** `QueryPerformanceCounter`를 사용하여 매우 정밀한 시간을 측정합니다.
- **델타 타임 (Delta Time, `dt`):** 이전 프레임에서 현재 프레임까지 걸린 시간(초 단위)입니다.
    - 예: 60 FPS라면 `dt`는 약 0.0166초.
    - 이동 거리 = `속도 * dt` 공식을 사용하면 프레임이 빨라지거나 느려져도 이동 거리는 일정해집니다.

### 주요 메서드

#### `Update()`
매 프레임 호출되어 `deltaTime`을 갱신합니다.

```cpp
void TimerManager::Update() {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // (현재 시간 - 이전 시간) / 주파수 = 흐른 시간(초)
    deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

    lastTime = currentTime;
}
```

#### 사용 예시

```cpp
// GameObject::Update(float dt) 내부
void Player::Update(float dt) {
    // 프레임에 상관없이 1초에 100픽셀 이동
    transform->position.x += 100.0f * dt;
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`InputManager`에서 `Down`, `Pressed`, `Up`을 구분해서 관리하는 이유는 무엇인가요? 만약 구분하지 않고 `Pressed`만 있다면 점프 기능을 구현할 때 어떤 문제가 생길까요?"
- "`Delta Time`을 곱해주지 않고 `position.x += 5`처럼 코드를 작성하면, 고사양 컴퓨터와 저사양 컴퓨터에서 게임 속도가 어떻게 달라지나요?"
- "`QueryPerformanceCounter`는 일반적인 `GetTickCount`나 `time()` 함수와 어떻게 다르며, 게임에서 이를 사용하는 이유는 무엇인가요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`InputManager`를 사용하여 '더블 클릭'을 감지하는 기능을 만들고 싶어. 마지막 클릭 시간을 저장하고 비교하는 로직을 작성해 줘."
- "특정 키(예: 'K')를 눌렀을 때만 디버그 모드를 켜고 끄는(Toggle) 기능을 구현해 줘. `ISKeyDown`을 사용해야 할까 `IsKeyPressed`를 사용해야 할까?"
- "`TimerManager`를 활용하여 현재 FPS(초당 프레임 수)를 화면 왼쪽 위에 텍스트로 띄우는 코드를 작성해 줘."
