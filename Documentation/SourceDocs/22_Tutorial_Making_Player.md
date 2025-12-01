# 22. 튜토리얼: 플레이어 만들기 (Tutorial: Making a Player)

## 소개 (Introduction)

지금까지 배운 내용들을 종합하여, 실제로 움직이고 점프하는 플레이어 캐릭터를 만드는 과정을 단계별로 설명합니다.
`FrameWork_2DAPI/z_SampleSource/Test_PlayerMoveCom.cpp` 파일을 참고하여 작성되었습니다.

**목표:**
1.  사용자 정의 컴포넌트(`Test_PlayerMoveCom`) 생성
2.  키보드 입력 처리 (`InputManager`)
3.  물리 엔진 연동 (`RigidbodyComponent`)

## 1단계: 컴포넌트 클래스 생성

먼저 `Component`를 상속받는 새로운 클래스를 만듭니다.

```cpp
// Test_PlayerMoveCom.h
#pragma once
#include "../Compoment/Component.h"

class RigidbodyComponent; // 전방 선언

class Test_PlayerMoveCom : public Component
{
public:
    Test_PlayerMoveCom();

    // 컴포넌트가 추가될 때 초기화
    virtual void Initialize_AddCompoment() override;

    // 매 프레임 실행될 로직
    virtual void Update(float dt) override;

private:
    float m_MoveSpeed = 200.0f; // 이동 속도
    float m_JumpPower = -500.0f; // 점프 힘 (위쪽이 -Y)

    // 다른 컴포넌트 참조
    RigidbodyComponent* m_LinkRigidBodyCom = nullptr;
};
```

## 2단계: 필요한 컴포넌트 가져오기

`Initialize_AddCompoment` 함수에서, 같은 게임 오브젝트에 붙어있는 `RigidbodyComponent`를 찾아옵니다.

```cpp
// Test_PlayerMoveCom.cpp
#include "Test_PlayerMoveCom.h"
#include "../Core/GameObject.h"
#include "../Compoment/RigidbodyComponent.h"

void Test_PlayerMoveCom::Initialize_AddCompoment()
{
    // 부모 클래스 초기화 호출
    __super::Initialize_AddCompoment();

    // 내 주인(owner)에게서 리지드바디 컴포넌트를 찾아서 저장
    m_LinkRigidBodyCom = owner->GetComponent<RigidbodyComponent>();
}
```

## 3단계: 이동 및 점프 로직 구현

`Update` 함수에서 입력을 감지하고 움직임을 처리합니다.

```cpp
#include "../Core/InputManager.h"

void Test_PlayerMoveCom::Update(float dt)
{
    // 1. 좌우 이동 (Transform 직접 이동)
    float moveX = 0.f;
    if (InputManager::IsKeyPressed(InputKey::A)) moveX = -1.f;
    if (InputManager::IsKeyPressed(InputKey::D)) moveX = 1.f;

    // transform은 Component 클래스에 이미 정의되어 있음 (owner->transform)
    transform->position.x += moveX * m_MoveSpeed * dt;

    // 2. 점프 (Rigidbody 힘 적용)
    if (InputManager::ISKeyDown(InputKey::Space))
    {
        if (m_LinkRigidBodyCom) // 리지드바디가 있을 때만
        {
            // 위쪽 방향으로 힘을 가함 (순간적인 힘)
            m_LinkRigidBodyCom->AddForce(0, m_JumpPower);
        }
    }
}
```

## 4단계: 씬에서 플레이어 조립하기

이제 `BaseGame`이나 `Scene`에서 실제로 이 플레이어를 생성해 봅니다.

```cpp
// BaseGame.cpp 또는 Scene 초기화 부분
void BaseGame::Test_InitScene()
{
    // 1. 빈 게임 오브젝트 생성
    GameObject* player = m_CurrentScene->CreateObject("Player");

    // 2. 이미지 추가 (선택)
    auto* img = player->AddComponent<ImageComponent>();
    img->ImageLoadImage(L"Assets/player.png");

    // 3. 물리 적용 (리지드바디)
    player->AddComponent<RigidbodyComponent>();

    // 4. 충돌체 적용 (박스 콜라이더)
    player->AddComponent<BoxCollider>(Vec2(0,0), 0, SizeF(50, 50));

    // 5. 우리가 만든 이동 스크립트 추가!
    player->AddComponent<Test_PlayerMoveCom>();
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`Initialize_AddCompoment`와 `Start` 함수의 차이점은 무엇이며, 왜 `GetComponent`는 주로 초기화 단계에서 호출하나요?"
- "`__super::Update(dt)`를 호출하는 이유는 무엇인가요? 부모 클래스의 기능을 유지하는 것의 중요성에 대해 설명해 줘."
- "이동 로직에서 `Transform`을 직접 수정하는 것과 `Rigidbody`의 속도(Velocity)를 수정하는 것의 차이는 무엇인가요? 물리 엔진과 충돌할 때 어떤 차이가 생기나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "플레이어가 점프 중일 때는 또 점프하지 못하도록(Double Jump 방지), '바닥 체크(Ground Check)' 로직을 추가해 줘."
- "총알을 발사하는 기능을 추가하고 싶어. 'F' 키를 누르면 플레이어 위치에서 총알 오브젝트를 생성(`CreateObject`)하고 앞으로 날아가게 하는 코드를 작성해 줘."
- "플레이어의 체력(HP)을 관리하고, 적과 충돌했을 때 체력이 깎이는 기능을 `OnCollisionEnter`를 사용하여 구현해 줘."
