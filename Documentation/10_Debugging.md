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

### 개념 이해
- "`DebugObjectManager`가 `m_DebugRectLineVec`에 미리 `DebugRectLineComponent` 객체들을 생성해두고, `DrawRectLine`이 호출될 때마다 `m_RectLineCount`를 증가시켜 순차적으로 재사용하는 '객체 풀링(Object Pooling)' 방식의 작동 원리에 대해 설명해 줘. 만약 요청이 풀의 크기를 초과하면 어떻게 동작하는지도 알려줘."
- "게임 루프의 맨 마지막에 `DebugObjectManager::ResetAllDebugObjects()`를 호출하여 `m_RectLineCount`와 같은 카운터들을 0으로 리셋하는데, 이 작업이 왜 '자동으로 지워지는' 효과를 만들어내는지 설명해 줘. 실제 메모리 해제가 일어나는 것은 아니라는 점을 명확히 해줘."
- "`UtilLoger::Log` 함수가 Windows API 함수인 `SetConsoleTextAttribute`를 사용하여 콘솔 텍스트 색상을 변경하는 원리를 설명해 줘. 이 방식이 C++ 표준 출력(`std::cout`)과 어떻게 함께 동작하는지 알려줘."
- "`DebugText` 객체는 `DebugObjectManager`에 의해 관리되는데, `DebugText`의 렌더링에 카메라 변환이 적용되는지 여부를 어떻게 제어할 수 있을까? (`DebugText`가 `TextComponent`와 유사한 내부 구조를 가졌다고 가정하고 설명해 줘)"

### 기능 구현 및 결과 도출
- "캐릭터의 이동 경로를 시각적으로 디버깅하고 싶어. `PlayerController`의 `Update` 메서드에서 매 프레임 캐릭터의 이전 위치와 현재 위치를 `DebugObjectManager::DrawLine`으로 연결하여 이동 궤적을 화면에 그리는 코드를 작성해 줘."
- "특정 `GameObject`의 위치(`transform->GetWorldPosition()`)와 이름(`GetName()`)을 매 프레임 화면에 표시하여 디버깅하고 싶어. `MyDebugInfo`라는 컴포넌트를 만들고, `Update` 메서드에서 `DebugObjectManager::DrawDebugText`를 호출하여 `owner`의 정보를 표시하는 코드를 작성해 줘."
- "콘솔에 너무 많은 로그가 출력되어 확인하기 어려울 때가 있어. `UtilLoger`에 로그 레벨 필터링 기능을 추가하고 싶어. `UtilLoger::SetLogLevel(E_LogLevel level)` 함수를 추가해서, 설정된 레벨 이상의 로그만 콘솔에 출력되도록 `Log` 함수를 수정하는 코드를 보여줘."
- "3D 공간에서의 레이캐스트를 디버깅하기 위해 시작점과 끝점을 받아 월드 좌표계의 선을 그리는 `DrawLine3D` 함수를 `DebugObjectManager`에 추가한다고 상상해 보자. 3D 좌표를 2D 화면 좌표로 변환(투영, Projection)하기 위해 어떤 행렬(View, Projection Matrix)이 필요하며, 변환된 좌표로 선을 그리기 위해 GDI+를 어떻게 사용해야 할지 그 과정을 설명해 줘."
