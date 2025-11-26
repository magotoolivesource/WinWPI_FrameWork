# 2. Component: GameObject에 생명을 불어넣는 부품

## 소개

`Component`는 `GameObject`에 부착되어 특정 기능과 동작을 구현하는 재사용 가능한 부품입니다. 렌더링, 물리, 사용자 입력 처리 등 게임에 필요한 거의 모든 로직은 `Component`를 통해 구현됩니다. `Component`는 스스로 존재할 수 없으며, 항상 `GameObject`에 속해야 합니다.

이 프레임워크의 모든 사용자 정의 컴포넌트는 `Component` 추상 클래스를 상속받아 만들어야 합니다.

## 핵심 개념

### `Component`의 역할

- **기능 분리:** 각 `Component`는 단일 책임을 가집니다. 예를 들어, `SpriteRenderer`는 렌더링만 담당하고, `PlayerController`는 사용자 입력만 처리합니다.
- **재사용성:** 잘 설계된 `Component`는 다양한 `GameObject`에 부착하여 재사용할 수 있습니다. 예를 들어, `Collider` 컴포넌트는 플레이어, 적, 장애물 등 충돌이 필요한 모든 `GameObject`에 사용할 수 있습니다.

### `owner`와 `transform`

`Component` 클래스에는 두 가지 중요한 멤버 변수가 있습니다.

- **`owner`:** 이 `Component`가 부착된 `GameObject`를 가리키는 포인터입니다. `owner`를 통해 `GameObject`의 다른 컴포넌트에 접근할 수 있습니다.
- **`transform`:** `owner` `GameObject`의 `Transform` 컴포넌트에 대한 바로 가기 포인터입니다. `Component`가 `GameObject`에 추가될 때 자동으로 초기화되어 위치, 회전, 크기 정보에 쉽게 접근할 수 있습니다.

### 생명주기(Lifecycle) 메서드

`Component`는 `GameObject`의 생명주기와 연결된 여러 가상 메서드를 가지고 있습니다. 이 메서드들을 재정의(override)하여 원하는 동작을 구현할 수 있습니다.

- **`Initialize_AddCompoment()`:** `Component`가 `GameObject`에 추가될 때 가장 먼저 호출됩니다. `transform` 포인터를 초기화하는 등의 기본 설정이 이루어집니다.
- **`Start()`:** `GameObject`가 활성화되고 첫 번째 `Update`가 호출되기 직전에 한 번 호출됩니다. 다른 컴포넌트에 대한 참조를 설정하거나 초기 상태를 설정하는 데 사용됩니다.
- **`Update(float dt)`:** 매 프레임마다 호출됩니다. `Component`의 핵심 로직이 실행되는 곳입니다.
- **`Render(HDC hdc)`:** `Update` 이후 매 프레임마다 호출되며, 화면에 무언가를 그리는 데 사용됩니다.
- **`RemoveCompoment()`:** `Component`가 `GameObject`에서 제거될 때 호출됩니다. 리소스를 해제하거나 정리하는 코드를 여기에 작성할 수 있습니다.
- **`SetEnabled(bool enabled)`:** `Component`를 활성화하거나 비활성화합니다. 비활성화된 `Component`는 `Update` 및 `Render`가 호출되지 않습니다.

## 사용자 정의 `Component` 만들기

### 1. `Component` 상속

새로운 C++ 클래스를 만들고 `Component`를 공개(public) 상속합니다.

```cpp
// SpriteRenderer.h
#pragma once
#include "Component.h"
#include <string>

class SpriteRenderer : public Component
{
public:
    void SetImage(const std::string& imagePath);

    // Component의 생명주기 메서드를 재정의합니다.
    virtual void Render(HDC hdc) override;

private:
    // 이 컴포넌트가 사용할 데이터
    std::string m_imagePath;
    HBITMAP m_hBitmap = nullptr;
};
```

### 2. 생명주기 메서드 구현

필요한 생명주기 메서드를 재정의하여 `Component`의 동작을 구현합니다.

```cpp
// SpriteRenderer.cpp
#include "SpriteRenderer.h"
#include "../Core/GameObject.h"
#include "Transform.h"

void SpriteRenderer::SetImage(const std::string& imagePath)
{
    m_imagePath = imagePath;
    // 실제 프로젝트에서는 AssetManager를 통해 이미지를 로드해야 합니다.
    m_hBitmap = (HBITMAP)LoadImageA(nullptr, imagePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void SpriteRenderer::Render(HDC hdc)
{
    if (!IsEnabled() || m_hBitmap == nullptr) return;

    // owner의 Transform 컴포넌트를 사용하여 위치를 가져옵니다.
    Vector2D position = transform->GetPosition();

    // 비트맵을 화면에 그립니다.
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, m_hBitmap);

    BITMAP bmpInfo;
    GetObject(m_hBitmap, sizeof(BITMAP), &bmpInfo);

    BitBlt(hdc, (int)position.x, (int)position.y, bmpInfo.bmWidth, bmpInfo.bmHeight, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}
```

### 3. `GameObject`에 추가

이제 새로 만든 `SpriteRenderer` 컴포넌트를 `GameObject`에 추가할 수 있습니다.

```cpp
// GameScene.cpp
#include "Core/GameObject.h"
#include "Compoment/SpriteRenderer.h"

void GameScene::Initialize()
{
    GameObject* player = new GameObject(this);
    player->SetName("Player");

    auto* renderer = player->AddComponent<SpriteRenderer>();
    renderer->SetImage("Assets/player.bmp");
}
```

## 참고 자료

- [Unity - Scripting](https://docs.unity3d.com/Manual/CreatingAndUsingScripts.html) (개념적으로 유사)
- [Unreal Engine - Components](https://docs.unrealengine.com/5.0/en-US/components-in-unreal-engine/)

## AI에게 할 수 있는 질문

### 개념 이해
- "`Component` 클래스에서 `owner`와 `transform` 멤버 변수가 왜 필요한지, 그리고 `owner->GetComponent<Transform>()` 대신 `transform`을 사용하는 것의 이점은 무엇인지 설명해 줘."
- "모든 컴포넌트가 `Component` 기본 클래스를 상속받아야 하는 이유는 무엇이야? 이것이 `GameObject`가 다양한 종류의 컴포넌트를 동일한 방식으로 관리할 수 있게 해주는 원리는 뭐야? (다형성)"
- "`Start()`와 `Initialize_AddCompoment()`의 호출 시점과 용도의 차이점을 설명해 줘. `Start`에서는 다른 컴포넌트를 `GetComponent`로 찾는 것이 안전하지만, `Initialize_AddCompoment`에서는 왜 위험할 수 있는지 알려줘."
- "컴포넌트를 비활성화(`SetEnabled(false)`)했을 때 `Update`와 `Render`만 중단되는 것인지, 아니면 컴포넌트 자체가 메모리에서 제거되는 것인지 설명해 줘."

### 기능 구현 및 결과 도출
- "`GameObject`가 파괴될 때(`RemoveCompoment` 호출 시) 특정 효과음을 재생하는 `SoundOnDestroy` 컴포넌트를 만들어보고 싶어. `RemoveCompoment` 메서드를 어떻게 오버라이드하고, 그 안에서 어떤 코드를 호출해야 하는지 보여줘."
- "`PlayerController` 컴포넌트의 `Start` 메서드에서, 같은 `GameObject`에 있는 `StatComponent`를 찾아서 `m_StatComponent`라는 멤버 변수에 저장해두고 싶어. `Start` 메서드에 어떤 코드를 작성해야 하는지 보여줘."
- "특정 아이템을 먹으면 5초 동안 플레이어의 이동 속도가 2배가 되는 기능을 구현하고 싶어. `PlayerController` 컴포넌트에 `EnableSpeedBoost(float duration)`라는 메서드를 만들고, 이 메서드가 `Move_Com`의 속도를 변경한 뒤 타이머를 사용해 5초 후에 원래 속도로 되돌리도록 코드를 작성해 줘."
- "컴포넌트의 `Update` 메서드에서 `owner->RemoveComponent<MyComponent>()`처럼 자기 자신을 제거하는 코드를 호출하면 어떤 문제가 발생할 수 있을까? 이런 경우 안전하게 컴포넌트를 제거하려면 어떻게 해야 할까?"

## PVSZ 예제: 컴포넌트의 실제 활용

`PVSZ` 프로젝트는 컴포넌트 기반 설계를 적극적으로 활용하여 `GameObject`에 다양한 기능을 부여합니다. 각 컴포넌트는 독립적인 책임을 가지며, 이들을 조합하여 복잡한 게임 객체를 만들어냅니다.

### `Move_Com`: 이동 기능

`Move_Com`은 `GameObject`에 간단한 등속 이동 기능을 추가하는 컴포넌트입니다.

```cpp
// Move_Com.h (간략화)
class Move_Com : public Component
{
public:
    virtual void Update(float dt) override;

    void SetMoveSpeed(Vec2 p_speed); // 이동 속도 설정
protected:
    Vec2 m_MoveSpeed = { 0.f, 0.f };
};

// Move_Com.cpp (간략화)
void Move_Com::Update(float dt)
{
    // owner의 Transform 컴포넌트에 접근하여 위치를 변경합니다.
    owner->transform->position.x += m_MoveSpeed.x * dt;
    owner->transform->position.y += m_MoveSpeed.y * dt;
}

// 좀비나 총알(Pea) GameObject에 이 컴포넌트를 추가하면,
// 해당 GameObject는 설정된 속도로 자동으로 움직이게 됩니다.
// 예:
// GameObject* pea = CreatePea();
// auto* move = pea->AddComponent<Move_Com>();
// move->SetMoveSpeed({ 150.0f, 0.0f }); // 오른쪽으로 초당 150픽셀 이동
```
이처럼 `Move_Com`은 '이동'이라는 단일 책임을 가지므로, 좀비, 총알, 움직이는 구름 등 이동이 필요한 모든 `GameObject`에 재사용될 수 있습니다.

### `Pea_Com`: 완두콩 총알의 행동 정의

`Pea_Com`은 완두콩 총알(`Pea`)의 모든 행동 로직을 담당합니다. `Pea_Com`은 `Move_Com`과 `Collider` 컴포넌트와 함께 작동합니다.

```cpp
// Pea_Com.h (간략화)
class Pea_Com : public Component
{
public:
    static Pea_Com* Create_PeaComObject(); // 완두콩 GameObject 생성 팩토리 메서드

protected:
    // Collider 컴포넌트가 다른 Collider와 충돌했을 때 호출되는 콜백 메서드
    void Collider_TriggerEnter(Collider* src_other, Collider* dest_other, void* p_data);
};

// Pea_Com.cpp (간략화)
void Pea_Com::Collider_TriggerEnter(Collider* src_other, Collider* dest_other, void* p_data)
{
    // 충돌한 상대방(dest_other)이 "Zombie" 태그를 가지고 있는지 확인합니다.
    if (dest_other->owner->HasTag("Zombie"))
    {
        // 좀비에게 데미지를 입히는 로직...

        // 자기 자신(총알)을 파괴합니다.
        owner->Destroy();
    }
}
```
`Pea_Com`은 '총알로서의 역할'에만 집중합니다. 이동은 `Move_Com`에, 충돌 감지는 `Collider`에 위임합니다. 이처럼 각 컴포넌트가 자신의 역할에만 충실하도록 설계하면 코드를 이해하고 유지보수하기가 훨씬 쉬워집니다.
