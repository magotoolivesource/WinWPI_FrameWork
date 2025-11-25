# 4. InputManager: 사용자 입력 처리

## 소개

`InputManager`는 키보드와 마우스 입력을 감지하고 관리하는 정적(static) 클래스입니다. 게임의 어느 곳에서든 `InputManager`의 메서드를 직접 호출하여 사용자의 입력 상태를 확인할 수 있으므로, `GameObject`나 `Component`에서 플레이어의 조작에 반응하는 코드를 쉽게 작성할 수 있습니다.

`InputManager`는 매 프레임 시작 시 `ResetAllKey()`를 호출하여 이전 프레임의 "방금 눌림" 및 "방금 떼어짐" 상태를 초기화해야 합니다. 이 작업은 보통 게임의 메인 루프에서 이루어집니다.

## 핵심 개념

### 입력 상태

`InputManager`는 세 가지 주요 입력 상태를 제공하여 다양한 종류의 상호작용을 구현할 수 있도록 합니다.

- **`ISKeyDown()` (방금 눌림):** 키가 해당 프레임에서 **처음으로 눌렸을 때** `true`를 반환합니다. 단발적인 행동(예: 점프, 총알 발사)을 구현하는 데 유용합니다.
- **`IsKeyPressed()` (누르고 있음):** 키가 **눌려 있는 동안** 계속해서 `true`를 반환합니다. 지속적인 행동(예: 캐릭터 이동, 무기 충전)을 구현하는 데 적합합니다.
- **`ISKeyUp()` (방금 떼어짐):** 키가 해당 프레임에서 **처음으로 떼어졌을 때** `true`를 반환합니다. 키를 놓는 순간에 발생하는 행동(예: 활시위 놓기, 메뉴 닫기)을 구현하는 데 사용됩니다.

### `InputKey` 열거형

`InputManager`는 `InputKey` 열거형(enum)을 제공하여 가상 키 코드를 직접 사용하는 대신 `InputKey::Space`, `InputKey::LeftArrow`, `InputKey::A`와 같이 명확하고 읽기 쉬운 이름으로 키를 참조할 수 있도록 합니다.

### 마우스 입력

마우스 입력도 키보드와 유사하게 세 가지 상태(`ISMouseDown`, `IsMouseLeftDown`(누르고 있음), `ISMouseUp`)를 확인할 수 있으며, `GetMousePosition()`을 통해 현재 마우스 커서의 화면 좌표를 얻을 수 있습니다.

## `InputManager` 사용법

### 키보드 입력 확인하기

`Component`의 `Update` 메서드 내에서 `InputManager`를 사용하여 키보드 입력을 확인하는 것이 일반적입니다.

```cpp
// PlayerController.cpp
#include "Core/InputManager.h"
#include "Core/GameObject.h"
#include "Compoment/Transform.h"

void PlayerController::Update(float dt)
{
    // --- 캐릭터 이동 (누르고 있는 동안) ---
    if (InputManager::IsKeyPressed(InputKey::LeftArrow))
    {
        owner->transform->position.x -= 100.0f * dt;
    }
    if (InputManager::IsKeyPressed(InputKey::RightArrow))
    {
        owner->transform->position.x += 100.0f * dt;
    }

    // --- 점프 (방금 눌렀을 때) ---
    if (InputManager::ISKeyDown(InputKey::Space))
    {
        // 점프 로직 실행
        Jump();
    }

    // --- 스킬 사용 (방금 떼었을 때) ---
    if (InputManager::ISKeyUp(InputKey::Q))
    {
        // 스킬 사용 로직 실행
        UseSkill();
    }
}
```

### 마우스 입력 확인하기

마우스 버튼 클릭 및 위치를 확인하는 방법입니다.

```cpp
// Gun.cpp
#include "Core/InputManager.h"

void Gun::Update(float dt)
{
    // 마우스 왼쪽 버튼을 방금 눌렀을 때 총알 발사
    if (InputManager::ISMouseDown(E_MOUSEETYPE::LEFT))
    {
        FireBullet();
    }

    // 마우스 오른쪽 버튼을 누르고 있는 동안 조준
    if (InputManager::IsMouseRightDown()) // IsKeyPressed와 유사
    {
        Aim();
    }

    // 마우스 커서 위치 가져오기
    POINT mousePos = InputManager::GetMousePosition();
    // mousePos.x, mousePos.y를 사용하여 무언가를 합니다.
}
```

### 메인 루프에서의 초기화

`InputManager`가 매 프레임마다 "방금 눌림" 및 "방금 떼어짐" 상태를 정확하게 감지하려면, 게임의 메인 루프 시작 부분에서 `ResetAllKey()`를 호출해야 합니다.

```cpp
// BaseGame.cpp
void BaseGame::GameLoop()
{
    // --- 루프 시작 ---

    // 이전 프레임의 입력 상태 초기화
    InputManager::ResetAllKey();

    // 메시지 펌프 (WinAPI 이벤트 처리)
    // ... PeekMessage, DispatchMessage ...
    // 이 과정에서 KeyDown, KeyUp 등의 이벤트가 InputManager에 전달됩니다.

    // 게임 로직 업데이트
    m_currentScene->UpdateLoop(dt);

    // 렌더링
    m_currentScene->Render(hdc);

    // --- 루프 끝 ---
}
```

## AI에게 할 수 있는 질문

- "`ISKeyDown`, `IsKeyPressed`, `ISKeyUp`의 차이점은 무엇이며, 각각 어떤 상황에서 사용해야 해?"
- "`InputManager`가 정적 클래스로 구현된 이유는 무엇이며, 이로 인해 얻는 장점과 단점은 무엇이야?"
- "게임 루프에서 `InputManager::ResetAllKey()`를 호출하는 것을 잊으면 어떤 문제가 발생할 수 있어?"
- "마우스 위치는 어떤 좌표계를 기준으로 반환되며, 이를 게임 월드 좌표로 변환하려면 어떻게 해야 할까?"
