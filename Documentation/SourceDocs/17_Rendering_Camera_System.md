# 17. 렌더링과 카메라 시스템 (Rendering & Camera System)

## 소개 (Introduction)

게임의 월드는 무한히 넓을 수 있지만, 모니터 화면은 크기가 제한되어 있습니다.
따라서 "게임 세상의 어디를 비출 것인가?"를 결정하는 **카메라(Camera)**와, "오브젝트를 어떻게 화면에 그릴 것인가?"를 담당하는 **렌더링(Rendering)** 시스템이 필수적입니다.

**관련 파일:**
- `FrameWork_2DAPI/Manager/CameraManager.h`, `CameraManager.cpp`
- `FrameWork_2DAPI/Compoment/Camera.h`, `Camera.cpp`
- `FrameWork_2DAPI/Compoment/ImageComponent.h`, `ImageComponent.cpp`

## 1. 좌표계 시스템 (Coordinate Systems)

게임을 만들 때 두 가지 좌표계를 이해해야 합니다.

- **월드 좌표 (World Space):** 게임 속 실제 위치입니다. 플레이어가 (1000, 500)에 있다면, 이는 월드 좌표입니다.
- **스크린 좌표 (Screen Space):** 모니터 화면상의 픽셀 위치입니다. 카메라가 (1000, 500)을 비추고 있다면, 플레이어는 화면 중앙(예: 400, 300)에 그려져야 합니다.

**카메라의 역할**은 월드 좌표를 스크린 좌표로 변환해주는 것입니다.

## 2. CameraSystem (카메라 시스템)

### CameraManager

게임 내의 모든 카메라를 관리하며, 특히 **메인 카메라(Main Camera)**를 제공하여 대부분의 렌더링에 사용됩니다.

- **Viewport Size:** 화면의 크기(해상도)를 관리합니다.
- **Main Camera:** `AddMainCamera()`를 통해 생성되며, `GetMainCamera()`로 어디서든 접근할 수 있습니다.

### Camera Component

실질적인 좌표 변환 계산을 담당합니다. `GameObject`에 컴포넌트로 부착되어 사용됩니다.

- **위치 (Position):** 카메라가 비추고 있는 월드 중심점입니다.
- **줌 (Zoom):** 화면의 확대/축소 비율입니다. (1.0 = 기본, 2.0 = 2배 확대)
- **회전 (Rotation):** 카메라의 기울기입니다.

#### 변환 행렬 (Transformation Matrix)
카메라는 GDI+의 `Matrix` 기능을 사용하여 변환을 수행합니다.

```cpp
// 1. 월드 좌표를 원점(0,0)으로 이동 (카메라 위치 기준)
matrix.Translate(-cameraPos.x, -cameraPos.y);

// 2. 회전 적용
matrix.Rotate(rotation);

// 3. 확대/축소 적용
matrix.Scale(zoom, zoom);

// 4. 화면 중앙으로 다시 이동 (Viewport Center)
matrix.Translate(screenCenter.x, screenCenter.y);
```
이 과정을 통해 "카메라가 오른쪽으로 가면, 물체들은 왼쪽으로 가는" 효과가 구현됩니다.

## 3. Rendering System (렌더링 시스템)

### ImageComponent

이미지(스프라이트)를 화면에 그리는 컴포넌트입니다.

#### 주요 기능
- **이미지 로드:** `Gdiplus::Image`를 사용하여 png, jpg 등을 불러옵니다.
- **그리기 영역 (Source Rect):** 큰 이미지에서 일부분만 잘라서 그릴 수 있습니다. (스프라이트 시트 애니메이션에 사용)
- **렌더링 파이프라인:** `Render(HDC hdc)` 함수에서 수행됩니다.

#### 렌더링 과정
1.  **Graphics 객체 생성:** GDI+ 그리 도구를 준비합니다.
2.  **변환 적용:**
    - 먼저 카메라의 변환 행렬을 적용하여 월드 좌표 -> 스크린 좌표로 바꿉니다.
    - 그 다음 오브젝트 자신의 변환(위치, 회전, 크기)을 적용합니다.
3.  **그리기:** 변환된 좌표계 위에 이미지를 그립니다.

```cpp
// ImageComponent::Render 내부 (개념 코드)
void ImageComponent::Render(HDC hdc) {
    Graphics graphics(hdc);

    // 1. 카메라 변환 적용 (월드 -> 스크린)
    Camera* mainCam = CameraManager::GetI()->GetMainCamera();
    mainCam->Begin_Update_GraphicsMatrix(graphics, transform);

    // 2. 이미지 그리기 (0,0 위치에 그리면 됨, 이미 좌표계가 변환되었으므로)
    graphics.DrawImage(m_Image, ...);

    // 3. 변환 복구
    mainCam->End_Update_GraphicsMatrix(graphics, transform);
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "월드 좌표를 스크린 좌표로 변환할 때, 왜 카메라 위치를 뺄셈(`-cameraPos`)하고 뷰포트 센터를 덧셈(`+screenCenter`)하는지 수학적으로 설명해 줘."
- "GDI+의 `Matrix` 클래스가 하는 역할은 무엇이며, 행렬 곱셈 순서가 달라지면 결과가 어떻게 변하나요?"
- "`ImageComponent`에서 `DrawImage`를 할 때 왜 오브젝트의 `position`이 아닌 `(0, 0)`이나 오프셋 위치에 그리는 건가요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Camera`를 따라다니는 'Smooth Follow' 기능을 구현하고 싶어. `Lerp`(선형 보간) 함수를 사용하여 카메라가 플레이어를 부드럽게 추적하는 코드를 작성해 줘."
- "화면이 흔들리는 'Camera Shake' 효과를 만들고 싶어. `Update` 함수에서 카메라 위치에 난수(Random) 값을 더해서 지진 효과를 내는 방법을 알려줘."
- "미니맵을 만들기 위해 메인 카메라 외에 두 번째 카메라를 추가하고, 화면 오른쪽 아래 작게 렌더링하는 방법을 설계해 줘."
