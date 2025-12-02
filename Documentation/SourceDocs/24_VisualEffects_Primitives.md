# 24. 시각 효과와 도형 (Visual Effects & Primitives)

## 소개 (Introduction)

게임 개발 중에는 복잡한 이미지(스프라이트) 외에도 간단한 선이나 사각형을 그려야 할 때가 많습니다.
예를 들어, 디버깅용 충돌 박스, 레이저 빔, 경로 표시, UI 테두리 등을 그릴 때 사용됩니다.
이 프레임워크는 이러한 도형을 그리기 위한 컴포넌트들을 제공합니다.

**관련 파일:**
- `FrameWork_2DAPI/Compoment/LineComponent.h`, `LineComponent.cpp`
- `FrameWork_2DAPI/Compoment/RectLineComponent.h`, `RectLineComponent.cpp`
*(폴더명 오타 주의: `Compoment` -> `Component`)*

## 1. 선 그리기 (LineComponent)

두 점 사이를 연결하는 선을 그립니다.

### 주요 속성
- **Start / End:** 선의 시작점과 끝점 좌표입니다.
- **Color:** 선의 색상 (Alpha, Red, Green, Blue)을 설정합니다.
- **Thickness:** 선의 굵기를 설정합니다.
- **Matrix Flags:**
    - `m_ISCameraMatrix`: 카메라 이동에 영향을 받을지 결정합니다. (true: 월드 좌표, false: 스크린 좌표)
    - `m_ISTransformMatrix`: `Transform` 컴포넌트의 위치/회전을 따를지 결정합니다.

```cpp
// 사용 예시: 레이저 빔
GameObject* laser = scene->CreateObject("Laser");
auto* line = laser->AddComponent<LineComponent>(
    PointF(0, 0), PointF(500, 0),
    Color(255, 255, 0, 0), // 빨간색
    5.0f // 굵기
);
```

## 2. 사각형 테두리 그리기 (RectLineComponent)

사각형의 외곽선을 그립니다. 충돌 박스(Collider)를 시각화하거나 UI 테두리를 만들 때 유용합니다.

### 주요 속성
- **Rect:** 그릴 사각형의 영역(x, y, width, height)입니다.
- **Color, Thickness:** 선의 색상과 굵기입니다.

```cpp
// 사용 예시: 선택 영역 표시
auto* box = obj->AddComponent<RectLineComponent>(
    RectF(0, 0, 100, 100),
    Color(255, 0, 255, 0), // 초록색
    2.0f
);
```

## 3. 재질 (Material)

**관련 파일:** `FrameWork_2DAPI/Core/Material.h`

`Material` 클래스는 렌더링에 필요한 이미지와 UV 좌표 정보를 담고 있는 데이터 구조체입니다.
현재는 주로 내부적인 데이터 전달용으로 사용되지만, 추후 쉐이더 설정이나 블렌딩 모드 등을 확장할 때 중요한 역할을 하게 됩니다.

- **Image:** 사용할 GDI+ 이미지 포인터.
- **UV Rect:** 이미지의 어느 부분을 사용할지 결정하는 텍스처 좌표.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`LineComponent`에서 `m_ISCameraMatrix`를 `false`로 설정하면 어떤 효과가 나나요? UI를 만들 때 어떻게 활용할 수 있을까요?"
- "GDI+에서 선을 그릴 때 `SmoothingMode` (안티앨리어싱)를 켜면 성능에 어떤 영향을 미치나요?"
- "`Material` 클래스가 단순히 이미지만 담는 것이 아니라 'UV 좌표'를 함께 담는 이유는 무엇인가요? 스프라이트 애니메이션과 어떤 관련이 있나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`LineComponent`를 사용하여 플레이어가 마우스로 드래그하는 궤적(Trail)을 그리는 기능을 구현해 줘. 최근 10개의 점을 저장해서 이어 그리는 방식은 어때?"
- "`RectLineComponent`를 사용하여 마우스가 오브젝트 위에 올라갔을 때(Hover)만 테두리가 생기는 기능을 만들어 줘."
- "원이 점점 커지면서 사라지는 '폭발 이펙트'를 만들고 싶어. `CircleLineComponent` 같은 것을 새로 만들어야 할까, 아니면 `Gdiplus::Graphics::DrawEllipse`를 어떻게 활용해야 할까?"
