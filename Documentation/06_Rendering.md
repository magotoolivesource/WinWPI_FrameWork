# 6. 렌더링 시스템: 게임 세계에 색을 입히다

## 소개

렌더링 시스템은 `GameObject`와 그 컴포넌트들을 화면에 시각적으로 표현하는 역할을 합니다. 이 프레임워크는 GDI+를 기반으로 하며, `ImageComponent`, `Camera`, `CameraManager` 세 가지 핵심 클래스를 통해 2D 렌더링을 처리합니다.

- **`ImageComponent`:** `GameObject`에 이미지를 표시하는 기능을 제공합니다.
- **`Camera`:** 게임 세계의 어느 부분을 화면에 보여줄지 결정하는 가상 카메라입니다.
- **`CameraManager`:** 게임 내의 모든 `Camera`를 관리하고, 주(Main) 카메라에 쉽게 접근할 수 있도록 합니다.

## 핵심 개념

### 렌더링 파이프라인 (간략화)

`GameObject`가 화면에 그려지기까지의 과정은 다음과 같습니다.

1.  **`Scene`의 `Render` 호출:** 게임 루프가 현재 `Scene`의 `Render` 메서드를 호출합니다.
2.  **`GameObject` 순회:** `Scene`은 정렬된 모든 `GameObject`를 순회하며 각각의 `Render` 메서드를 호출합니다.
3.  **`Component` 순회:** `GameObject`는 자신에게 부착된 모든 `Component`를 순회하며 각각의 `Render` 메서드를 호출합니다.
4.  **`ImageComponent`의 렌더링:** `ImageComponent`의 `Render` 메서드가 호출되면 다음과 같은 일이 발생합니다.
    a. `CameraManager`로부터 메인 `Camera`를 가져옵니다.
    b. `Gdiplus::Graphics` 객체를 생성합니다.
    c. `Camera`의 `Begin_Update_GraphicsMatrix`를 호출하여 `Graphics` 객체에 월드-뷰 변환 행렬을 적용합니다. 이 변환은 `GameObject`의 위치, 회전, 크기와 `Camera`의 위치, 회전, 줌 값을 모두 고려합니다.
    d. 변환된 `Graphics` 객체를 사용하여 이미지를 그립니다.
    e. `Camera`의 `End_Update_GraphicsMatrix`를 호출하여 `Graphics` 객체의 상태를 원래대로 복원합니다.

### 월드 좌표와 화면 좌표

- **월드 좌표 (World Space):** 게임 세계 내의 절대적인 위치입니다. 모든 `GameObject`의 `Transform`은 월드 좌표를 기준으로 위치를 가집니다.
- **화면 좌표 (Screen Space):** 모니터 화면의 픽셀 좌표입니다. 왼쪽 상단이 (0, 0)입니다.

`Camera`의 가장 중요한 역할은 `GameObject`의 월드 좌표를 최종적으로 렌더링될 화면 좌표로 변환하는 것입니다.

### `ImageComponent`와 스프라이트 시트

`ImageComponent`는 `SetDrawRect` 메서드를 통해 스프라이트 시트(하나의 이미지에 여러 프레임의 애니메이션이나 여러 이미지가 포함된 것)의 특정 부분만 잘라내어 그릴 수 있는 기능을 제공합니다. 이는 메모리를 절약하고 애니메이션을 효율적으로 관리하는 데 매우 유용합니다.

## 사용법

### 이미지 렌더링하기

`GameObject`에 `ImageComponent`를 추가하고 이미지 파일을 로드하면 화면에 이미지가 표시됩니다.

```cpp
// GameScene.cpp
#include "Core/GameObject.h"
#include "Compoment/ImageComponent.h"

void GameScene::InitSettings()
{
    GameObject* player = CreateObject("Player");
    player->transform->SetWorldPosition(100, 150);

    auto* renderer = player->AddComponent<ImageComponent>();
    // ImageManager를 통해 이미지를 로드합니다.
    renderer->ImageLoadImage(L"Assets/player_sprite.png");
}
```

### 스프라이트의 일부만 사용하기

`SetDrawRect`를 사용하여 이미지의 특정 영역만 그리도록 설정할 수 있습니다. `SetSize`는 해당 영역을 화면에 얼마나 크게 그릴지 결정합니다.

```cpp
// player_spritesheet.png 파일에 32x32 크기의 캐릭터 이미지가 여러 개 있다고 가정
GameObject* player = CreateObject("Player");
auto* renderer = player->AddComponent<ImageComponent>();
renderer->ImageLoadImage(L"Assets/player_spritesheet.png");

// 스프라이트 시트의 (32, 0) 위치에서 32x32 크기의 영역을 잘라냅니다. (두 번째 캐릭터)
renderer->SetDrawRect(32, 0, 64, 32);
// 잘라낸 이미지를 화면에 64x64 크기로 그립니다.
renderer->SetSize(64, 64);
```

### 카메라 제어하기

`CameraManager`를 통해 메인 `Camera`에 접근하여 위치를 이동하거나 줌(Zoom) 효과를 적용할 수 있습니다.

```cpp
// CameraController.cpp
#include "Manager/CameraManager.h"
#include "Compoment/Camera.h"
#include "Core/InputManager.h"

void CameraController::Update(float dt)
{
    Camera* mainCamera = CameraManager::GetI()->GetMainCamera();
    if (mainCamera == nullptr) return;

    // 'W' 키를 누르면 카메라를 위로 이동
    if (InputManager::IsKeyPressed(InputKey::W))
    {
        Vec2 currentPos = mainCamera->GetWorldPos();
        mainCamera->SetWorldPosition(currentPos.x, currentPos.y - 100.0f * dt);
    }

    // '+' 키를 누르면 줌 인
    if (InputManager::ISKeyDown(InputKey::Add))
    {
        mainCamera->SetZoom(mainCamera->GetZoom() * 1.1f);
    }

    // '-' 키를 누르면 줌 아웃
    if (InputManager::ISKeyDown(InputKey::Subtract))
    {
        mainCamera->SetZoom(mainCamera->GetZoom() * 0.9f);
    }
}
```

## AI에게 할 수 있는 질문

- "렌더링 파이프라인에서 `Camera`의 뷰 변환 행렬이 적용되는 시점은 언제이며, 이 행렬은 어떤 정보들을 바탕으로 계산돼?"
- "`ImageComponent`의 `SetDrawRect`와 `SetSize` 메서드는 각각 어떤 역할을 하며, 두 메서드를 함께 사용하면 어떤 효과를 얻을 수 있어?"
- "카메라의 `Zoom` 값을 변경하면 화면에 보이는 `GameObject`들에 어떤 변화가 생기는지 설명해 줘."
- "GDI+의 `Graphics::MultiplyTransform` 메서드는 어떤 순서로 행렬을 곱하는 것이 중요한 이유는 무엇이야?"
