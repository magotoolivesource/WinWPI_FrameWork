# 3. Scene 관리: 게임 월드의 무대

## 소개

`Scene`은 게임의 레벨, 메뉴 화면, 로딩 화면 등 독립적인 게임 공간을 구성하는 컨테이너입니다. 각 `Scene`은 자체적인 `GameObject` 그룹을 가지고 있으며, 이들을 관리하고 업데이트, 렌더링하는 역할을 합니다. 플레이어가 게임을 진행하면서 다른 레벨로 이동하거나 메뉴로 돌아가는 것은 활성화된 `Scene`을 전환하는 것을 의미합니다.

현재 프레임워크에는 별도의 `SceneManager`가 없으므로, `Scene`의 생성과 전환은 게임의 메인 루프에서 직접 관리해야 합니다.

## 핵심 개념

### `Scene`의 역할

- **`GameObject` 컨테이너:** `Scene`은 내부에 여러 `GameObject`를 소유하고 관리합니다. `Scene`이 활성화되면 그 안에 속한 모든 `GameObject`들이 업데이트되고 렌더링됩니다.
- **게임 흐름 제어:** `Scene`을 전환함으로써 게임의 상태(예: 메인 메뉴 -> 게임 플레이 -> 게임 오버)를 변경할 수 있습니다.
- **렌더링 순서 관리:** `Scene`은 `GameObject`의 `Transform` 컴포넌트에 설정된 `depth` 값을 기준으로 렌더링 순서를 정렬합니다. `depth` 값이 작을수록 먼저 그려져 뒤에 배치되고, 클수록 나중에 그려져 앞에 배치됩니다.

### `Scene`의 생명주기

`Scene`은 다음과 같은 주요 생명주기 메서드를 가집니다.

- **`InitSettings()`:** `Scene`이 처음 로드될 때 호출됩니다. 이 `Scene`에서 사용할 `GameObject`를 생성하고 초기화하는 코드를 여기에 작성합니다.
- **`UpdateLoop(float dt)`:** 매 프레임마다 호출됩니다. 현재 `Scene`에 속한 모든 활성 `GameObject`의 `Update` 메서드를 호출합니다.
- **`Render(HDC hdc)`:** `UpdateLoop` 이후 매 프레임마다 호출됩니다. 현재 `Scene`에 속한 모든 활성 `GameObject`의 `Render` 메서드를 호출합니다.
- **`Release()`:** `Scene`이 전환되거나 게임이 종료될 때 호출됩니다. 이 `Scene`에서 사용된 모든 `GameObject`와 리소스를 정리하는 코드를 여기에 작성합니다.

## 사용자 정의 `Scene` 만들기

### 1. `Scene` 상속

새로운 C++ 클래스를 만들고 `Scene`을 공개(public) 상속합니다.

```cpp
// TitleScene.h
#pragma once
#include "Scene/Scene.h"

class TitleScene : public Scene
{
public:
    // Scene의 생명주기 메서드를 재정의합니다.
    virtual void InitSettings() override;
    virtual void Release() override;
};
```

### 2. 생명주기 메서드 구현

`InitSettings`에서 `GameObject`를 생성하고, `Release`에서 정리합니다.

```cpp
// TitleScene.cpp
#include "TitleScene.h"
#include "Core/GameObject.h"
#include "Compoment/SpriteRenderer.h"
#include "Compoment/UIButton.h"

void TitleScene::InitSettings()
{
    // 배경 이미지 GameObject 생성
    GameObject* background = CreateObject("Background");
    auto* bgRenderer = background->AddComponent<SpriteRenderer>();
    bgRenderer->SetImage("Assets/title_background.bmp");
    background->transform->SetDepth(-1.0f); // 배경이므로 뒤에 배치

    // 시작 버튼 GameObject 생성
    GameObject* startButton = CreateObject("StartButton");
    auto* buttonComponent = startButton->AddComponent<UIButton>();
    buttonComponent->SetButtonImage("Assets/start_button.bmp");

    // 버튼 클릭 시 "GameScene"으로 전환하는 람다 함수를 등록합니다.
    buttonComponent->SetOnClick([&]() {
        // 이 부분은 SceneManager가 구현되어야 더 간단해집니다.
        // 현재는 메인 루프에서 Scene을 전환해야 합니다.
        // 예: game->ChangeScene(new GameScene());
    });
}

void TitleScene::Release()
{
    // Scene이 자동으로 모든 GameObject를 정리하므로
    // 특별히 수동으로 해제할 리소스가 없다면 비워둘 수 있습니다.
    Scene::Release();
}
```

## `Scene` 전환하기 (현재 구조에서의 제안)

현재 프레임워크에는 `SceneManager`가 없으므로 `BaseGame` 클래스(또는 게임의 메인 클래스)에서 현재 `Scene`을 포인터로 관리하고, 필요할 때 이를 교체하는 방식으로 `Scene` 전환을 구현할 수 있습니다.

```cpp
// BaseGame.h
#include "Scene/Scene.h"
#include <memory>

class BaseGame
{
    // ...
private:
    std.unique_ptr<Scene> m_currentScene;
public:
    void ChangeScene(Scene* newScene);
};

// BaseGame.cpp
void BaseGame::ChangeScene(Scene* newScene)
{
    if (m_currentScene)
    {
        m_currentScene->Release();
    }
    m_currentScene.reset(newScene);
    m_currentScene->InitSettings();
}

// 게임 초기화 시
void BaseGame::Initialize()
{
    // ...
    ChangeScene(new TitleScene());
}

// 게임 루프
void BaseGame::GameLoop()
{
    // ...
    if (m_currentScene)
    {
        m_currentScene->UpdateLoop(dt);
        m_currentScene->Render(hdc);
    }
    // ...
}
```

## AI에게 할 수 있는 질문

- "`Scene`이 `GameObject`의 렌더링 순서를 결정하는 기준은 무엇이며, 어떻게 제어할 수 있어?"
- "`Scene`에서 `GameObject`를 생성할 때 `new GameObject()` 대신 `CreateObject()`를 사용해야 하는 이유는 무엇이야?"
- "현재 프레임워크에 `SceneManager`가 없는 상황에서 `Scene` 전환을 구현하려면 어떻게 해야 할까?"
- "`Scene`의 `Release()` 메서드가 호출될 때 어떤 일들이 발생하는지 설명해 줘."

## PVSZ 예제: 실제 게임 씬(Scene) 구성

`PVSZ` 프로젝트의 `Stage01` 클래스는 `Scene`을 상속받아 실제 게임의 첫 번째 레벨을 구현한 좋은 예시입니다. `Stage01`은 `InitSettings` 메서드 안에서 레벨에 필요한 모든 `GameObject`를 생성하고 초기화합니다.

```cpp
// Stage01.h (간략화)
class Stage01 : public Scene
{
public:
    // Scene의 생명주기 메서드를 재정의합니다.
    virtual void InitSettings() override;

protected:
    // 레벨의 각 부분을 초기화하는 헬퍼(Helper) 함수들
    void InitBGCom();      // 배경 이미지 생성
    void InitHeaderCard(); // 상단 식물 선택 UI 생성
    void InitWaveData();   // 좀비 웨이브 정보 설정
};
```

```cpp
// Stage01.cpp (간략화)
void Stage01::InitSettings()
{
    // Scene의 기본 초기화 함수를 먼저 호출합니다.
    __super::InitSettings();

    // 좀비 웨이브 데이터를 설정하고, StageWaveManager를 통해 좀비 생성을 예약합니다.
    InitWaveData();

    // 배경 이미지를 생성합니다.
    InitBGCom();

    // 화면 상단에 표시될 식물 선택 UI를 생성합니다.
    InitHeaderCard();
}

void Stage01::InitBGCom()
{
    // "BG"라는 이름의 GameObject를 생성합니다. CreateObject는 Scene의 멤버 함수입니다.
    auto gbobj = this->CreateObject("BG");

    // ImageComponent를 추가하고 배경 이미지를 로드합니다.
    auto bgcom = gbobj->AddComponent<ImageComponent>();
    bgcom->ImageLoadImage(L"Assets/Images/Background/background1.jpg");

    // 배경이 다른 모든 오브젝트보다 뒤에 그려지도록 depth 값을 낮게 설정합니다.
    bgcom->transform->SetDepth(-100);
}
```

이처럼 `Scene` 클래스는 게임의 특정 단계나 화면에 필요한 모든 요소를 한 곳에서 생성하고 관리하는 '무대 감독'과 같은 역할을 합니다. 새로운 레벨을 추가하고 싶다면, `Scene`을 상속받는 새로운 클래스(예: `Stage02`)를 만들고 `InitSettings`를 채워 넣으면 됩니다.
