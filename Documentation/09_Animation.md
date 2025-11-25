# 9. 애니메이션 시스템: GameObject에 움직임을 부여하기

## 소개

애니메이션 시스템은 여러 장의 이미지를 연속적으로 보여줌으로써 `GameObject`가 움직이는 것처럼 보이게 만드는 기능입니다. 이 프레임워크는 `ImageAni_Component`를 통해 스프라이트 시트(Sprite Sheet) 기반의 2D 애니메이션을 구현합니다.

애니메이션을 재생하려면 `GameObject`에 `ImageComponent`와 `ImageAni_Component`가 모두 부착되어 있어야 합니다. `ImageAni_Component`는 애니메이션 데이터를 관리하고, `ImageComponent`는 실제로 이미지를 화면에 그리는 역할을 합니다.

## 핵심 개념

### 애니메이션 클립(Animation Clip)

`ImageAni_Component`는 여러 개의 프레임 정보가 모인 '애니메이션 클립'을 재생하는 역할을 합니다. 각 프레임은 `DrawImageRectInfoData` 객체에 의해 정의되며, 다음과 같은 정보를 포함합니다.

-   **이미지 경로 (`m_ImagePath`):** 이 프레임에 사용할 이미지 파일의 경로입니다.
-   **지속 시간 (`m_DurationSec`):** 이 프레임이 화면에 표시될 시간(초)입니다.
-   **스프라이트 영역 (`drawRect`):** 이미지 파일에서 잘라낼 사각형 영역입니다.
-   **표시 크기 (`drawWidth`, `drawHeight`):** 잘라낸 이미지를 화면에 그릴 크기입니다.
-   **오프셋 (`OffsetPosX`, `OffsetPosY`):** 이미지의 표시 위치를 미세 조정하기 위한 값입니다.

### 애니메이션 재생 과정

1.  **프레임 데이터 추가:** `AddDrawImageInfo` 메서드를 사용하여 `ImageAni_Component`에 여러 프레임의 정보를 순서대로 추가합니다.
2.  **데이터 조정:** `AdjustImageInfo` 또는 `SetNAdjustImageInfoTotalFrame` 메서드를 호출하여 추가된 프레임 정보들을 내부적으로 처리하고 이미지를 미리 로드합니다.
3.  **재생:** `Play()` 메서드를 호출하면 애니메이션 재생이 시작됩니다.
4.  **업데이트:** `Update` 메서드는 매 프레임마다 경과 시간을 확인하고, 현재 프레임의 지속 시간이 다 되면 다음 프레임으로 전환합니다.
5.  **이미지 컴포넌트 업데이트:** 프레임이 전환될 때마다, `ImageAni_Component`는 연결된 `ImageComponent`의 `SetImage`와 `SetDrawRect` 메서드를 호출하여 화면에 표시되는 이미지를 변경합니다.

## `ImageAni_Component` 사용법

### 1. `GameObject` 설정

애니메이션을 적용할 `GameObject`에 `ImageComponent`와 `ImageAni_Component`를 모두 추가합니다.

```cpp
#include "Core/GameObject.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/ImageAni_Component.h"

GameObject* player = CreateObject("Player");
player->AddComponent<ImageComponent>();
auto* animator = player->AddComponent<ImageAni_Component>();
```

### 2. 애니메이션 프레임 정의

`AddDrawImageInfo`를 사용하여 애니메이션을 구성할 각 프레임의 정보를 추가합니다. 보통은 별도의 헬퍼(Helper) 함수나 데이터 파일로 이 과정을 자동화합니다.

```cpp
void SetupPlayerAnimation(ImageAni_Component* animator)
{
    std::wstring spritePath = L"Assets/player_walk.png";

    // 걷기 애니메이션 (4 프레임)
    // 각 프레임은 0.15초 동안 지속됩니다.
    // player_walk.png 파일에서 32x64 크기의 영역을 순서대로 잘라냅니다.
    animator->AddDrawImageInfo(0.15f, spritePath, 64, 128, {0, 0, 32, 64}, 0, 0);
    animator->AddDrawImageInfo(0.15f, spritePath, 64, 128, {32, 0, 64, 64}, 0, 0);
    animator->AddDrawImageInfo(0.15f, spritePath, 64, 128, {64, 0, 96, 64}, 0, 0);
    animator->AddDrawImageInfo(0.15f, spritePath, 64, 128, {96, 0, 128, 64}, 0, 0);

    // 프레임 정보를 내부적으로 처리하고 이미지를 로드합니다.
    animator->AdjustImageInfo();
}
```

### 3. 애니메이션 제어

애니메이션을 설정한 후 `Play()`, `Pause()`, `SetLoopTime()` 등의 메서드로 재생을 제어할 수 있습니다.

```cpp
// player_walk.png 파일에 32x64 크기의 캐릭터 이미지가 여러 개 있다고 가정
void StartPlayer()
{
    GameObject* player = CreateObject("Player");
    player->AddComponent<ImageComponent>();
    auto* animator = player->AddComponent<ImageAni_Component>();

    SetupPlayerAnimation(animator);

    // 애니메이션을 무한 반복하도록 설정합니다.
    animator->SetLoopTime(-1);

    // 애니메이션 재생을 시작합니다.
    animator->Play();
}
```

### PVSZ 예제에서의 활용

`PVSZ` 예제 프로젝트의 좀비나 식물들은 모두 이 애니메이션 시스템을 사용하여 구현되었습니다. 예를 들어, `Zombie` 클래스는 `ImageAni_Component`를 사용하여 걷는 애니메이션, 공격하는 애니메이션 등을 관리하고, 좀비의 상태에 따라 적절한 애니메이션 클립을 재생합니다.

```cpp
// (가상 코드) Zombie.cpp
void Zombie::Walk()
{
    m_state = ZombieState::Walk;
    // 미리 설정된 걷기 애니메이션 클립(ImageAni_Component)으로 교체하고 재생
    SwitchAnimation(m_walkAnimation);
}

void Zombie::Attack()
{
    m_state = ZombieState::Attack;
    // 공격 애니메이션으로 교체하고 재생
    SwitchAnimation(m_attackAnimation);
}
```

## AI에게 할 수 있는 질문

- "`ImageAni_Component`가 `ImageComponent`와 분리되어 있는 이유는 무엇이며, 두 컴포넌트가 어떻게 상호작용하는지 설명해 줘."
- "애니메이션의 각 프레임 지속 시간을 모두 동일하게 설정하는 `SetNAdjustImageInfoTotalFrame` 메서드는 어떤 경우에 유용하게 사용될 수 있어?"
- "애니메이션의 재생 속도를 2배로 빠르게 하려면 `SetWeight_AnimationVal` 값을 어떻게 설정해야 해?"
- "`ImageAni_Component`를 확장하여 애니메이션 재생이 끝났을 때 특정 함수를 호출하는 '완료 이벤트' 기능을 추가하려면 어떻게 구현해야 할까?"

## PVSZ 예제: 상태에 따른 애니메이션 전환

`PVSZ` 프로젝트의 `NormalZombie` 클래스는 `ImageAni_Component`를 활용하여 좀비의 상태(예: 걷기, 공격, 죽음)에 따라 다른 애니메이션을 재생하는 좋은 예시입니다.

### 1. 애니메이션 설정 함수 분리

`NormalZombie` 클래스에는 각 상태에 맞는 애니메이션을 설정하는 별도의 함수들이 있습니다. 이렇게 하면 코드를 관리하기가 더 쉬워집니다.

```cpp
// NormalZombie.h (간략화)
class NormalZombie : public AutoZombie<NormalZombie>
{
    // ...
protected:
    // 각 상태에 대한 애니메이션을 설정하는 함수들
    void InitSettingBaseAnimations(); // 걷기 애니메이션 (기본 상태)
    void Set_AttackAnimations();      // 공격 애니메이션
    void Set_DieAnimations();         // 죽는 애니메이션

    // 이 좀비가 사용하는 ImageAni_Component에 대한 포인터
    ImageAni_Component* m_LinkImaeAniCom = nullptr;
};
```

### 2. 걷기 애니메이션 설정

`InitSettingBaseAnimations` 함수는 좀비가 처음 생성될 때 호출되며, 걷는 애니메이션 클립을 설정합니다.

```cpp
// NormalZombie.cpp (간략화)
void NormalZombie::InitSettingBaseAnimations()
{
    // owner GameObject에서 ImageAni_Component를 가져와 멤버 변수에 저장해 둡니다.
    m_LinkImaeAniCom = this->owner->AddComponent<ImageAni_Component>();

    // 걷기 애니메이션은 총 22개의 프레임으로 구성됩니다.
    wstring path = L"";
    for (size_t i = 0; i < 22; i++)
    {
        // "Assets/Images/Zombies/Zombie/Zombie_1.png", ".../Zombie_2.png", ...
        path = format(L"Assets/Images/Zombies/Zombie/Zombie_{}.png", i + 1);
        m_LinkImaeAniCom->AddDrawImageInfo(0.2f, path, ...); // 프레임 정보 추가
    }

    // 전체 애니메이션 길이를 3초로 맞추고, 각 프레임의 지속 시간을 자동으로 계산합니다.
    m_LinkImaeAniCom->SetNAdjustImageInfoTotalFrame(3.f);
    m_LinkImaeAniCom->SetLoopTime(-1); // 무한 반복
    m_LinkImaeAniCom->Play();          // 즉시 재생 시작
}
```

### 3. 상태 변경 시 애니메이션 전환

좀비의 체력이 0이 되어 죽는 상태가 되면, `SetDieEvent` 함수가 호출됩니다. 이 함수는 `Set_DieAnimations`를 호출하여 현재 재생 중인 걷기 애니메이션을 중단하고 죽는 애니메이션으로 교체합니다.

```cpp
// NormalZombie.cpp (간략화)
void NormalZombie::SetDieEvent()
{
    m_ISDie = true;
    SetMove(false); // 이동 중지

    // 현재 애니메이션을 지우고 죽는 애니메이션으로 교체합니다.
    Set_DieAnimations();

    // ... 일정 시간 후 GameObject를 파괴하는 타이머 설정 ...
}

void NormalZombie::Set_DieAnimations()
{
    // 기존의 모든 프레임 정보를 지웁니다.
    m_LinkImaeAniCom->ResetAllDatasNClear();

    // 죽는 애니메이션(BoomDie)은 20개의 프레임으로 구성됩니다.
    wstring path = L"";
    for (size_t i = 0; i < 20; i++)
    {
        path = format(L"Assets/Images/Zombies/Zombie/BoomDie_{}.png", i + 1);
        m_LinkImaeAniCom->AddDrawImageInfo(0.2f, path, ...);
    }

    m_LinkImaeAniCom->SetNAdjustImageInfoTotalFrame(2.f);
    m_LinkImaeAniCom->SetLoopTime(1); // 한 번만 재생
    m_LinkImaeAniCom->Play();
}
```
이처럼 각 상태에 맞는 애니메이션 클립을 미리 함수로 만들어 두고, 게임 상황에 따라 `ResetAllDatasNClear()`로 초기화한 뒤 새로운 클립을 설정하고 재생하는 방식으로 다양한 애니메이션을 표현할 수 있습니다.
