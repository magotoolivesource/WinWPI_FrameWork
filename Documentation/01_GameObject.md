# 1. GameObject: 게임 내 모든 개체의 기초

## 소개

`GameObject`는 게임 세계에 존재하는 모든 개체(캐릭터, 총알, UI 요소 등)의 기본 단위입니다. `GameObject` 자체는 텅 빈 컨테이너와 같으며, 여기에 다양한 `Component`를 추가하여 원하는 기능과 동작을 구현할 수 있습니다. 예를 들어, 플레이어 캐릭터 `GameObject`는 플레이어의 위치와 방향을 나타내는 `Transform` 컴포넌트, 화면에 플레이어를 그리는 `SpriteRenderer` 컴포넌트, 사용자 입력을 처리하는 `PlayerController` 컴포넌트를 가질 수 있습니다.

이 프레임워크는 컴포넌트 기반 아키텍처를 사용하여 유연하고 확장 가능한 게임 개발을 지원합니다.

## 핵심 개념

### 컴포넌트 기반 아키텍처

이 프레임워크는 상속 대신 구성을 사용하는 컴포넌트 기반 설계를 따릅니다. 즉, 거대한 클래스 계층 구조를 만드는 대신, 재사용 가능한 독립적인 `Component`를 만들어 `GameObject`에 추가합니다.

- **유연성:** 필요에 따라 `GameObject`에 컴포넌트를 추가하거나 제거하여 기능을 쉽게 변경할 수 있습니다.
- **재사용성:** 하나의 컴포넌트(예: `PhysicsBody`)를 다양한 유형의 `GameObject`(예: 플레이어, 적, 아이템)에서 재사용할 수 있습니다.

### `Transform` 컴포넌트

모든 `GameObject`는 생성 시 자동으로 `Transform` 컴포넌트를 갖습니다. 이 컴포넌트는 `GameObject`의 위치(Position), 회전(Rotation), 크기(Scale)를 저장하며, 게임 세계에서 `GameObject`의 시각적 표현에 매우 중요합니다.

- `Transform`은 `GameObject`에서 제거할 수 없습니다.
- `GameObject`의 모든 공간 정보는 `transform` 멤버 변수를 통해 액세스할 수 있습니다.

### 생명주기(Lifecycle)

`GameObject`와 그 컴포넌트는 다음과 같은 주요 생명주기 메서드를 따릅니다. 이 메서드들은 `Scene`에 의해 자동으로 호출됩니다.

- **`Start()`:** `GameObject`가 활성화되고 처음으로 업데이트되기 직전에 한 번 호출됩니다. 주로 초기화 코드에 사용됩니다.
- **`Update(float dt)`:** 매 프레임마다 호출됩니다. 게임 로직, 입력 처리, 상태 변경 등 대부분의 작업이 여기에서 수행됩니다. `dt`는 이전 프레임과의 시간 간격(Delta Time)입니다.
- **`Render(HDC hdc)`:** `Update()`가 끝난 후 매 프레임마다 호출됩니다. `GameObject`와 그 컴포넌트를 화면에 그리는 코드가 여기에서 실행됩니다.

## `GameObject` 사용법

### `GameObject` 생성하기

`GameObject`는 일반적으로 `Scene` 객체를 통해 생성됩니다.

```cpp
// GameScene.cpp
#include "Core/GameObject.h"
#include "Scene/Scene.h"

void GameScene::Initialize()
{
    // "Player"라는 이름의 새로운 GameObject를 생성합니다.
    GameObject* player = new GameObject(this);
    player->SetName("Player");
}
```

### 컴포넌트 추가하기

`AddComponent<T>()` 템플릿 메서드를 사용하여 `GameObject`에 새로운 컴포넌트를 추가할 수 있습니다.

```cpp
// Player.cpp
#include "Core/GameObject.h"
#include "Compoment/SpriteRenderer.h"
#include "Compoment/PlayerController.h"

void CreatePlayer()
{
    GameObject* player = new GameObject(this); // 'this'는 Scene을 가리킵니다.
    player->SetName("Player");

    // SpriteRenderer 컴포넌트를 추가하여 플레이어를 화면에 그립니다.
    auto* renderer = player->AddComponent<SpriteRenderer>();
    renderer->SetImage("player_sprite.png"); // 렌더링할 이미지 설정

    // PlayerController 컴포넌트를 추가하여 사용자 입력을 처리합니다.
    player->AddComponent<PlayerController>();
}
```

### 컴포넌트 접근하기

`GetComponent<T>()` 템플릿 메서드를 사용하여 `GameObject`에 추가된 컴포넌트에 접근할 수 있습니다.

```cpp
// PlayerController.cpp
#include "Core/GameObject.h"
#include "Compoment/SpriteRenderer.h"

void PlayerController::Update(float dt)
{
    // owner는 이 컴포넌트가 속한 GameObject를 가리킵니다.
    SpriteRenderer* renderer = owner->GetComponent<SpriteRenderer>();
    if (renderer != nullptr)
    {
        // SpriteRenderer를 사용하여 무언가를 합니다.
    }

    // Transform 컴포넌트는 항상 존재합니다.
    Transform* transform = owner->transform;
    transform->position.x += 10.0f * dt; // 오른쪽으로 이동
}
```

### 컴포넌트 제거하기

`RemoveComponent<T>()` 템플릿 메서드를 사용하여 `GameObject`에서 컴포넌트를 제거할 수 있습니다.

```cpp
// 예시
void RemovePlayerSprite(GameObject* player)
{
    // 플레이어의 SpriteRenderer 컴포넌트를 제거합니다.
    player->RemoveComponent<SpriteRenderer>();
}
```

## 고급 주제: 지연된 컴포넌트 관리

프레임워크는 안정성을 위해 컴포넌트의 추가 및 제거를 즉시 처리하지 않고 지연시킵니다.

- **추가:** `AddComponent`로 추가된 컴포넌트는 "대기" 목록에 들어갑니다. 현재 프레임의 업데이트 루프가 시작되기 직전에 모든 대기 중인 컴포넌트의 `Start()` 메서드가 호출되고 활성 상태가 됩니다.
- **제거:** `RemoveComponent`로 제거된 컴포넌트는 "제거 대기" 목록에 들어갑니다. 현재 프레임의 모든 렌더링이 완료된 후에 실제로 메모리에서 해제됩니다.

이러한 방식은 게임 루프 중에 컬렉션을 수정할 때 발생할 수 있는 일반적인 버그(예: 반복자 무효화)를 방지합니다.

## 참고 자료

- [Component-Based Architecture (Game Programming Patterns)](https://gameprogrammingpatterns.com/component.html)
- [Unity - GameObjects](https://docs.unity3d.com/Manual/GameObjects.html) (개념적으로 유사)

## AI에게 할 수 있는 질문

- "`GameObject`와 `Component`의 관계를 상속 대신 구성을 사용하는 이유와 함께 설명해 줘."
- "`Transform` 컴포넌트가 모든 `GameObject`에 자동으로 추가되는 이유는 무엇이야?"
- "컴포넌트의 추가 및 제거를 즉시 처리하지 않고 지연시키는 이유는 무엇이며, 이로 인해 얻는 이점은 무엇이야?"
- "`GetComponent`를 호출했을 때 요청한 타입의 자식 클래스 컴포넌트도 찾아줄 수 있는지 설명해 줘."

## PVSZ 예제: `GameObject`의 실제 활용

`PVSZ` 예제 프로젝트에서는 `GameObject`를 상속받거나 `GameObject`에 컴포넌트를 추가하는 방식으로 게임의 모든 캐릭터와 오브젝트를 구현합니다.

### 좀비(Zombie) 구현 예시

`NormalZombie` 클래스는 좀비의 기본 클래스인 `Base_ZombiActor`를 상속받습니다. `Base_ZombiActor`는 `GameObject`를 상속받아 좀비의 공통적인 속성(체력, 속도 등)과 기능(피격 처리, 상태 변경)을 미리 구현해 둔 클래스입니다.

```cpp
// Base_ZombiActor.h (간략화)
class Base_ZombiActor : public GameObject
{
public:
    // ... 좀비 공통 기능 ...
    virtual void TakeDamage(float damage);
protected:
    float m_health;
    float m_moveSpeed;
};

// NormalZombie.h (간략화)
// AutoZombie는 템플릿 클래스이며, 내부적으로 Base_ZombiActor를 상속받습니다.
class NormalZombie : public AutoZombie<NormalZombie>
{
public:
    virtual void InitSettings() override;
    // ... NormalZombie만의 고유한 로직 ...
};
```
이처럼 `GameObject`를 상속받아 게임의 핵심 액터(Actor)를 정의함으로써, 코드의 재사용성을 높이고 각 캐릭터의 역할을 명확하게 구분할 수 있습니다.

### 식물(Plant) 구현 예시

`Plant_PeaShooter`는 `GameObject`를 직접 상속받는 대신, `Component`로서 구현되었습니다. 그리고 `Create_PeaShooterObject`라는 정적(static) 팩토리 메서드를 통해 완두콩 슈터 `GameObject`를 생성합니다.

```cpp
// Plant_PeaShooter.h (간략화)
class Plant_PeaShooter : public Component
{
public:
    // 완두콩 슈터 GameObject를 생성하고 필요한 컴포넌트를 모두 추가하여 반환합니다.
    static Plant_PeaShooter* Create_PeaShooterObject();

    // ... 완두콩 슈터의 행동 로직 (예: 총알 발사) ...
protected:
    void FireBullet();
};

// Plant_PeaShooter.cpp (상상)
Plant_PeaShooter* Plant_PeaShooter::Create_PeaShooterObject()
{
    GameObject* obj = new GameObject(); // Scene을 통해 생성해야 함
    obj->SetName("PeaShooter");

    obj->AddComponent<ImageComponent>();
    obj->AddComponent<ImageAni_Component>();

    // Plant_PeaShooter 컴포넌트 자체를 추가하여 행동 로직을 부여합니다.
    auto* peaShooterLogic = obj->AddComponent<Plant_PeaShooter>();

    return peaShooterLogic;
}
```
이 방식은 `GameObject`의 생성과 초기화 과정을 하나의 함수로 캡슐화하여, 씬(Scene)에서 식물을 더 간편하게 생성할 수 있도록 도와줍니다.
