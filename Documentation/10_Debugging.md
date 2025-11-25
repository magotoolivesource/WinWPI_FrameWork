# 10. 디버깅 도구: 보이지 않는 것을 보게 하다

## 소개

디버깅은 개발 과정에서 발생하는 문제를 해결하고 게임의 내부 상태를 파악하는 데 필수적인 작업입니다. 이 프레임워크는 게임 화면에 정보를 시각적으로 표시하고 콘솔에 로그를 출력하는 등 강력한 디버깅 도구를 제공하여 개발을 지원합니다.

- **`DebugObjectManager`:** 게임 화면에 선, 사각형, 텍스트와 같은 디버깅 정보를 일시적으로 그리는 싱글턴(Singleton) 클래스입니다. 충돌 범위, AI의 경로, 특정 좌표 등을 시각화하는 데 사용됩니다.
- **`UtilLoger`:** 개발자 콘솔에 색상으로 구분된 로그 메시지를 출력하는 정적(static) 클래스입니다. 변수의 값, 함수의 호출 순서, 이벤트 발생 여부 등을 확인하는 데 유용합니다.

## `DebugObjectManager`: 화면에 정보 시각화하기

`DebugObjectManager`는 매 프레임마다 그렸던 내용을 자동으로 지우므로, `Update` 메서드에서 안심하고 호출하여 원하는 정보를 계속해서 화면에 표시할 수 있습니다.

### 핵심 개념

- **일시적인 드로잉:** `DebugObjectManager`를 통해 그려진 모든 것은 다음 프레임이 시작되기 전에 자동으로 지워집니다. 따라서 매 프-레임마다 그리고 싶은 정보는 `Update` 메서드 내에서 호출해야 합니다.
- **객체 재활용:** `DebugObjectManager`는 내부적으로 디버깅 객체(선, 사각형 등)의 풀(Pool)을 관리합니다. 매번 새로운 객체를 생성하고 파괴하는 대신, 비활성화된 객체를 재활용하여 성능 저하를 최소화합니다.
- **전역 제어:** `DebugObjectManager::GetI()->SetAllDebug(false)`를 호출하면 모든 디버깅 그리기를 한 번에 비활성화할 수 있습니다. 최종 빌드 시 디버깅 정보를 쉽게 제거하는 데 유용합니다.

### 사용법

#### 충돌 범위(Collider) 시각화

`CollisionManager`의 `DrawColliders`와 같은 기능은 내부적으로 `DebugObjectManager`를 사용하여 모든 충돌체의 외곽선을 화면에 그려줍니다.

```cpp
// BaseGame.cpp의 Render 메서드
void BaseGame::Render(HDC p_hdc, RECT& p_clientRect)
{
    // ...
    // 모든 충돌체의 외곽선을 그립니다.
    CollisionManager::GetI()->DrawColliders(p_hdc);
    // ...
}
```

#### 레이캐스트(Raycast) 시각화

`DebugObjectManager`를 직접 호출하여 광선(Ray)을 선으로 그릴 수 있습니다.

```cpp
// Weapon.cpp의 Update 메서드
void Weapon::Update(float dt)
{
    PointF start = owner->transform->GetWorldPosition();
    PointF end = GetFireDirection() * 1000.0f; // 1000 유닛 길이의 광선

    // 광선을 빨간색 선으로 그립니다.
    DebugObjectManager::Instance().DrawLine(start, end, 1.0f, Gdiplus::Color(255, 255, 0, 0));

    // ... 레이캐스트 충돌 처리 로직 ...
}
```

#### FPS와 같은 정보 표시

화면의 특정 위치에 텍스트를 표시하여 실시간 정보를 확인할 수 있습니다.

```cpp
// BaseGame.cpp의 Update 메서드
void BaseGame::Update()
{
    // ...
    float fps = m_pTimerManager->GetFPS();
    std::wstring msg = std::format(L"{:.0f} FPS", fps);
    Vec2 pos(10, 10); // 화면 왼쪽 상단

    // FPS를 노란색 텍스트로 그립니다.
    DebugObjectManager::Instance().DrawDebugText(msg, pos, Gdiplus::Color(255, 255, 255, 0));
}
```

## `UtilLoger`: 콘솔에 정보 출력하기

`UtilLoger`는 코드의 특정 지점을 지났는지 확인하거나, 변수의 값을 실시간으로 추적하고 싶을 때 유용합니다.

### 사용법

`UtilLoger::Log()` 정적 메서드를 호출하여 콘솔에 메시지를 출력할 수 있습니다. 로그 레벨(`INFO`, `WARN`, `ERROR`)에 따라 색상이 다르게 표시되어 가독성을 높여줍니다.

```cpp
#include "Core/UtilLoger.h"

void Player::TakeDamage(int damage)
{
    m_health -= damage;

    // 정보(INFO) 레벨 로그: 일반적인 정보 출력
    UtilLoger::Log(L"플레이어가 " + std::to_wstring(damage) + L"의 데미지를 받았습니다.");

    if (m_health < 30)
    {
        // 경고(WARN) 레벨 로그: 주의가 필요한 상황
        UtilLoger::Log(L"플레이어의 체력이 부족합니다!", E_LogLevel::E_WARN);
    }

    if (m_health <= 0)
    {
        // 에러(ERROR) 레벨 로그: 심각한 문제나 중요한 상태 변화
        UtilLoger::Log(L"플레이어가 사망했습니다.", E_LogLevel::E_ERROR);
        Die();
    }
}
```

## AI에게 할 수 있는 질문

- "`DebugObjectManager`가 매 프레임 디버깅 객체를 새로 생성하는 대신 재활용하는 방식을 사용하는 이유는 무엇이며, 이로 인한 성능상의 이점은 무엇이야?"
- "게임의 특정 `GameObject`가 왜 화면에 보이지 않는지 디버깅하고 싶을 때 `DebugObjectManager`를 어떻게 활용할 수 있을까?"
- "`UtilLoger`를 사용하여 특정 함수의 호출 횟수를 추적하려면 코드를 어떻게 작성해야 해?"
- "`DebugText`는 기본적으로 월드 좌표를 기준으로 텍스트를 그리는데, 카메라의 움직임과 상관없이 항상 화면의 특정 위치에 텍스트를 표시하려면 어떻게 해야 할까?"
