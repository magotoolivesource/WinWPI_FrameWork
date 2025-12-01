# 18. UI 시스템 (UI System)

## 소개 (Introduction)

UI(User Interface)는 버튼, 텍스트, 입력창 등 플레이어와 게임이 상호작용하는 수단입니다.
게임 월드에 존재하는 오브젝트와 달리, UI는 보통 화면에 고정되어 있거나 카메라의 움직임에 영향을 받지 않는 경우가 많습니다.

**관련 파일:**
- `FrameWork_2DAPI/UICompoment/Button.h`, `Button.cpp`
- `FrameWork_2DAPI/UICompoment/TextComponent.h`, `TextComponent.cpp`
*(폴더명 오타 주의: `UICompoment` -> `UIComponent`)*

## 1. 버튼 (Button)

마우스 입력을 받아 이벤트를 발생시키는 컴포넌트입니다.

### 핵심 기능

#### 클릭 감지 (`ISClick`)
마우스 포인터가 버튼 영역 안에 있는지 판별합니다.
단순히 좌표만 비교하는 것이 아니라, 버튼이 회전하거나 확대/축소되었을 때도 정확히 클릭을 감지하기 위해 **역행렬(Inverse Matrix)**을 사용합니다.

1.  **변환 행렬 구성:** 버튼의 위치, 회전, 크기 정보를 담은 행렬을 만듭니다.
2.  **역행렬 계산:** 이 행렬을 뒤집습니다(Invert).
3.  **마우스 좌표 변환:** 화면상의 마우스 좌표에 역행렬을 곱합니다.
4.  **판정:** 변환된 마우스 좌표가 버튼의 원래 크기(0,0 ~ width, height) 안에 들어오는지 확인합니다.

#### 콜백 함수 (Callback Function)
버튼이 클릭되었을 때 실행할 동작을 `std::function`을 사용하여 지정합니다.

```cpp
// 사용 예시
GameObject* btnObj = scene->CreateObject("StartButton");
auto* btn = btnObj->AddComponent<Button>([](Button* b) {
    printf("게임 시작!");
});
```

#### 상태 관리
- **Normal:** 평상시 상태.
- **Hover:** 마우스가 버튼 위에 있을 때.
- **Pressed:** 마우스를 누르고 있을 때.
각 상태에 따라 버튼의 색상을 다르게 렌더링하여 사용자에게 피드백을 줍니다.

## 2. 텍스트 (TextComponent)

GDI+를 사용하여 화면에 문자열을 그리는 컴포넌트입니다.

### 핵심 기능

- **속성 설정:** 폰트 종류(`Arial`, `Consolas` 등), 크기, 색상, 정렬 방식(좌측, 중앙, 우측)을 설정할 수 있습니다.
- **월드/스크린 렌더링:** `m_ISCameraMatrix` 플래그를 통해 텍스트가 게임 월드에 존재할지(캐릭터 이름표 등), 화면에 고정될지(점수판 등) 결정할 수 있습니다.

```cpp
// 사용 예시
TextComponent* text = obj->AddComponent<TextComponent>();
text->SetText(L"Score: 100");
text->SetFont(L"Consolas", 24.0f);
text->SetFontColor(255, 255, 0, 0); // 빨간색 (Alpha, R, G, B)
```

## 3. UI 렌더링 파이프라인

UI 컴포넌트들도 `Render(HDC hdc)` 함수를 통해 그려집니다.
일반적으로 UI는 다른 게임 오브젝트보다 **나중에(위에)** 그려져야 합니다. 이를 위해 씬(Scene)에서 Z-Order를 아주 낮게 설정하거나, 별도의 UI 레이어로 관리하는 것이 좋습니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "버튼 클릭을 감지할 때 왜 단순히 `if (mouseX > x && ...)`를 하지 않고 복잡하게 '역행렬'을 사용하나요?"
- "UI 좌표계와 월드 좌표계는 어떻게 다른가요? `CameraManager`가 UI 렌더링에 어떤 영향을 미치나요?"
- "`std::function`과 람다(Lambda)식을 사용하여 버튼 이벤트를 처리하는 방식의 장점은 무엇인가요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Button` 클래스에 이미지를 입히고 싶어. `ImageComponent`와 연동하여 마우스 오버 시 이미지가 바뀌는 'ImageButton'을 만들어 줘."
- "`TextComponent`를 사용하여 타이핑 효과(글자가 한 글자씩 나타나는 연출)를 주는 스크립트를 작성해 줘."
- "체력바(HP Bar)를 만들고 싶어. 현재 체력 비율에 따라 사각형의 너비가 줄어드는 `ProgressBar` UI 컴포넌트를 설계해 줘."
