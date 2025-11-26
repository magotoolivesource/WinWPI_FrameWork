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

### 개념 이해
- "`Camera::GetCameraWorldMatrix()` 내부에서 뷰 행렬을 계산할 때, GDI+ 행렬 연산(Scale, Rotate, Translate)이 어떤 순서로 적용되는지, 그리고 이 순서가 왜 중요한지 설명해 줘. 만약 순서를 바꾸면 어떤 시각적 문제가 발생할까?"
- "`ImageComponent`의 `Render` 함수에서 `graphics.MultiplyTransform(&transformmat)`를 먼저 호출하고 `graphics.MultiplyTransform(&campmat)`를 나중에 호출하고 있어. 이 두 행렬 곱셈의 순서가 최종적으로 `(ObjectWorldMatrix * CameraViewMatrix)` 연산을 수행하는 것과 동일한지 행렬 연산의 원리에 기반하여 설명해 줘."
- "카메라의 `Zoom` 값을 2.0으로 설정하면 `GetCameraWorldMatrix` 내부에서는 `Scale(0.5, 0.5)`를 호출하는데, 왜 확대 효과를 내기 위해 스케일 값을 역수(1/zoom)로 적용하는지 뷰 변환의 원리와 연관지어 설명해 줘."
- "`ImageComponent`의 `SetDrawRect`는 스프라이트 시트의 '어느 부분'을 사용할지 결정하고, `SetSize`는 그것을 화면에 '얼마나 크게' 그릴지 결정한다고 했는데, 이 두 가지를 조합해서 9분할(9-slicing) UI 이미지를 렌더링하는 원리를 설명하고, 이 프레임워크에서 구현하려면 `ImageComponent`를 어떻게 수정해야 할지 아이디어를 제시해 줘."

### 기능 구현 및 결과 도출
- "미니맵(Minimap)을 구현하고 싶어. 화면 우측 상단에 게임 월드의 축소판을 보여주는 두 번째 카메라를 추가하려면 `CameraManager`와 렌더링 파이프라인을 어떻게 수정해야 할까? 두 카메라의 렌더링 결과를 화면의 다른 영역에 각각 그리는 방법을 단계별로 설명해 줘."
- "`GameObject`가 피해를 입었을 때 0.1초 동안 빨간색으로 깜박이는 효과를 주고 싶어. GDI+의 `ColorMatrix`를 사용한다고 가정할 때, `ImageComponent`에 `TintColor`와 같은 속성을 추가하고 `Render` 함수에서 이를 적용하는 코드를 작성해 줘."
- "화면 전체에 적용되는 후처리 효과(Post-processing effect), 예를 들어 회색조(Grayscale) 효과를 구현하고 싶어. 모든 렌더링이 끝난 후 화면 전체 비트맵에 접근해서 픽셀 색상을 변경하려면, `BaseGame`의 렌더링 루프를 어떻게 수정해야 하는지 코드로 보여줘."
- "플레이어 캐릭터가 화면의 특정 영역(예: 중앙 80%)을 벗어나면 카메라가 플레이어를 따라가도록 만들고 싶어. `Camera`를 제어하는 `CameraController` 컴포넌트의 `Update` 메서드에서 플레이어의 화면 좌표를 계산하고, 이 좌표가 영역을 벗어났는지 판단하여 카메라 위치를 부드럽게 이동시키는 코드를 작성해 줘."
