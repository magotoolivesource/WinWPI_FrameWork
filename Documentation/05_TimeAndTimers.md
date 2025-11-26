# 5. 시간 및 타이머 관리: 게임의 심장 박동

## 소개

안정적이고 일관된 게임 경험을 제공하기 위해서는 시간 관리가 매우 중요합니다. 이 프레임워크는 두 가지 주요 클래스를 통해 시간과 타이머를 관리합니다.

- **`TimerManager`:** 프레임 간의 시간 간격(`DeltaTime`)을 정밀하게 측정하고, 현재 초당 프레임 수(FPS)를 계산합니다.
- **`UtilTimerManager`:** "3초 후에 이 함수를 실행해 줘" 또는 "매 프레임마다 이 함수를 호출해 줘"와 같이 시간 기반 이벤트를 예약하고 관리하는 싱글턴(Singleton) 클래스입니다.

## 핵심 개념

### `DeltaTime`의 중요성

`DeltaTime`은 이전 프레임을 완료하는 데 걸린 시간입니다. 컴퓨터의 성능에 따라 프레임 속도(FPS)는 달라질 수 있지만, `DeltaTime`을 사용하면 게임 로직이 프레임 속도에 영향을 받지 않고 일관되게 실행되도록 할 수 있습니다.

**잘못된 예 (프레임 종속적):**
```cpp
// 매 프레임마다 1픽셀씩 이동
// 빠른 컴퓨터에서는 매우 빠르게, 느린 컴퓨터에서는 매우 느리게 움직입니다.
position.x += 1;
```

**올바른 예 (프레임 독립적):**
```cpp
// 초당 100픽셀의 속도로 이동
// 컴퓨터 성능과 관계없이 항상 동일한 속도로 움직입니다.
float speed = 100.0f;
position.x += speed * (float)TimerManager::GetI()->GetDeltaTime();
```

### `UtilTimerManager`

`UtilTimerManager`는 콜백 함수를 사용하여 시간 기반 이벤트를 처리합니다. 이를 통해 복잡한 시간 관련 로직을 `Update` 메서드에 직접 구현하지 않고도 깔끔하게 관리할 수 있습니다.

- **원샷 타이머:** 지정된 시간이 지난 후 한 번만 콜백 함수를 실행합니다. (예: 수류탄 폭발)
- **반복 타이머:** 지정된 시간 간격으로 콜백 함수를 반복적으로 실행합니다. (예: 3초마다 체력 회복)
- **업데이트 타이머:** 매 프레임마다 콜백 함수를 실행합니다. `Update` 메서드와 유사하지만, 특정 로직을 분리하여 관리하고 싶을 때 유용합니다.

## `TimerManager`와 `UtilTimerManager` 사용법

### `DeltaTime` 사용하기

게임의 메인 클래스(`BaseGame`)가 `TimerManager`의 인스턴스를 소유하고 매 프레임 `Update()`를 호출합니다. 이 `DeltaTime` 값은 게임 루프를 통해 모든 `GameObject`와 `Component`의 `Update(float dt)` 메서드에 전달됩니다.

```cpp
// PlayerController.cpp
void PlayerController::Update(float dt)
{
    float moveSpeed = 200.0f;
    if (InputManager::IsKeyPressed(InputKey::RightArrow))
    {
        // dt를 곱하여 프레임 속도에 독립적인 이동을 구현합니다.
        owner->transform->position.x += moveSpeed * dt;
    }
}
```

### `UtilTimerManager`로 이벤트 예약하기

`UtilTimerManager`는 싱글턴이므로 `UtilTimerManager::GetI()`를 통해 어디서든 접근할 수 있습니다.

#### 1. 3초 후에 메시지를 출력하는 타이머

```cpp
#include "Core/UtilTimerManager.h"
#include "Core/UtilLoger.h"

void StartGame()
{
    UtilTimerManager::GetI()->AddTimer(
        3.0f,       // 3초 후에 실행
        nullptr,    // 매 프레임 업데이트 콜백은 사용하지 않음
        [](UtilTimer* timer, void* data) {
            // 3초가 지나면 이 코드가 실행됩니다.
            UtilLoger::Log(L"3초가 지났습니다!");
        },
        nullptr,    // 콜백에 전달할 사용자 데이터 (필요 시 사용)
        0           // 반복 횟수 (0 또는 1은 한 번만 실행)
    );
}
```

#### 2. 1초마다 체력을 10씩 회복하는 무한 반복 타이머

```cpp
void ActivateHealthRegen()
{
    int playerHealth = 50;

    UtilTimerManager::GetI()->AddTimer(
        1.0f,       // 1초 간격
        nullptr,
        [this, &playerHealth](UtilTimer* timer, void* data) {
            playerHealth += 10;
            UtilLoger::Log(L"체력 회복! 현재 체력: %d", playerHealth);
        },
        nullptr,
        -1          // -1은 무한 반복을 의미
    );
}
```

#### 3. 매 프레임마다 오브젝트를 회전시키는 타이머

```cpp
void RotateObject(GameObject* objectToRotate)
{
    UtilTimerManager::GetI()->AddUpdateTimer(
        [objectToRotate](UtilTimer* timer, void* data) {
            // 매 프레임마다 이 코드가 실행됩니다.
            float rotationSpeed = 90.0f; // 초당 90도
            // GetElapsed()는 타이머가 시작된 후 경과된 총 시간을 반환합니다.
            // 여기서는 매 프레임 업데이트이므로, DeltaTime을 직접 가져와야 합니다.
            // 이 예제는 개념 설명용이며, 실제로는 Component의 Update를 사용하는 것이 좋습니다.
        },
        nullptr
    );
}
```

### 타이머 중지 및 제거

`AddTimer` 또는 `AddUpdateTimer`를 호출하면 고유한 ID(정수)가 반환됩니다. 이 ID를 사용하여 나중에 타이머를 중지하거나 완전히 제거할 수 있습니다.

```cpp
int myTimerId = UtilTimerManager::GetI()->AddTimer(5.0f, nullptr, someCallback);

// ... 나중에 ...

// 타이머를 즉시 중지하고 제거합니다.
UtilTimerManager::GetI()->RemoveTimer(myTimerId);
```

## AI에게 할 수 있는 질문

### 개념 이해
- "`DeltaTime`을 사용하지 않고 `position.x += 1;`과 같이 코드를 작성하면 왜 '프레임 종속적'이 되는지 컴퓨터 성능(FPS)과 관련하여 설명해 줘. 60FPS 환경과 30FPS 환경에서 1초 동안 `GameObject`가 이동하는 거리가 어떻게 달라지는지 구체적인 숫자를 들어 비교해 줘."
- "`UtilTimerManager`의 `Update` 메서드에서 `timers` 벡터를 순회하면서 `timer->Update(deltaTime)`을 호출하는데, 만약 타이머의 콜백 함수 안에서 새로운 타이머를 추가(`AddTimer`)하면 어떤 일이 발생할까? 다음 프레임부터 즉시 업데이트되는지, 아니면 현재 프레임의 `Update` 루프에 포함되는지 알려줘."
- "타이머의 콜백 함수로 람다(lambda) 함수를 사용할 때, 외부 변수를 값으로 캡처(`[=]`)하는 것과 참조로 캡처(`[&]`)하는 것의 차이점은 무엇이며, 어떤 경우에 참조 캡처가 위험할 수 있는지 구체적인 예를 들어 설명해 줘. (예: 지역 변수 캡처)"
- "`AddTimer`의 `repeatcount` 인자가 0일 때와 1일 때는 동일하게 한 번만 실행되는데, -1일 때는 왜 무한 반복이 되는지 `UtilTimer`의 `Update` 메서드 내부 로직을 통해 설명해 줘."

### 기능 구현 및 결과 도출
- "게임에 '일시정지' 기능을 추가하고 싶어. `TimerManager`를 수정해서 `TimeScale`이라는 개념(예: `m_TimeScale = 1.0f`)을 도입하고, `GetDeltaTime`이 `deltaTime * m_TimeScale`을 반환하도록 만들고 싶어. `TimeScale`을 0으로 만들면 모든 게임 내 시간 흐름과 `UtilTimerManager`가 멈추게 되는지, 그리고 UI 애니메이션처럼 일시정지와 상관없이 움직여야 하는 요소는 어떻게 처리해야 하는지 알려줘."
- "3초에 걸쳐 `GameObject`를 A 지점에서 B 지점으로 부드럽게 이동(선형 보간, Lerp)시키는 기능을 `UtilTimerManager`를 사용해서 구현하고 싶어. `AddTimer`의 업데이트 콜백(`p_updatecallback`)을 사용하여 매 프레임 `GameObject`의 위치를 갱신하는 전체 코드를 작성해 줘."
- "`UtilTimerManager`에 등록된 모든 타이머를 한 번에 중지시키는 `PauseAllTimers()`와 다시 재개하는 `ResumeAllTimers()` 함수를 추가하고 싶어. `UtilTimerManager` 클래스에 어떤 멤버 변수와 로직을 추가해야 하는지 코드로 보여줘."
- "활을 쏠 때, 키를 누르고 있는 시간에 비례해서 `ImageComponent`의 색상이 점차 붉게 변하는 시각적 피드백을 주고 싶어. `AddTimer`의 업데이트 콜백을 사용해서, 타이머의 경과 시간(`GetElapsed`)에 따라 `ImageComponent`의 색상을 변경하는 코드를 작성하는 방법을 알려줘."
