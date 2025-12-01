# 19. 애니메이션 시스템 (Animation System)

## 소개 (Introduction)

정적인 이미지만으로는 생동감 있는 게임을 만들기 어렵습니다.
`ImageAni_Component`는 여러 장의 이미지(또는 스프라이트 시트의 여러 영역)를 순차적으로 보여주어, 캐릭터가 걷거나 공격하는 등의 움직임을 표현하는 컴포넌트입니다.

**관련 파일:**
- `FrameWork_2DAPI/Compoment/ImageAni_Component.h`, `ImageAni_Component.cpp`
*(폴더명 오타 주의: `Compoment` -> `Component`)*

## 핵심 개념 (Core Concepts)

- **프레임 애니메이션 (Frame Animation):** 필름처럼 여러 장의 그림을 빠르게 교체하여 움직이는 것처럼 보이게 하는 기법입니다.
- **스프라이트 시트 (Sprite Sheet):** 캐릭터의 모든 동작을 하나의 큰 이미지 파일에 모아둔 것입니다. 애니메이션 컴포넌트는 이 큰 이미지의 일부분(Rect)을 잘라서 보여줍니다.
- **애니메이션 데이터 (`DrawImageRectInfoData`):** 각 프레임이 어떤 이미지를 쓰고, 화면의 어디를 잘라서 보여줄지, 그리고 얼마 동안 보여줄지(Duration)에 대한 정보를 담고 있습니다.

## 주요 기능 및 메서드 (Key Features & Methods)

### `ImageAni_Component`

`ImageComponent`와 연동하여 실제 렌더링할 이미지를 매 프레임 교체해주는 컨트롤러 역할을 합니다.

#### 1. 애니메이션 등록 (`AddDrawImageInfo`)

각 프레임의 정보를 등록합니다.

```cpp
// 사용 예시
auto* ani = obj->AddComponent<ImageAni_Component>();

// "Walk"라는 이름으로 애니메이션 프레임 추가
// (이름, 지속시간, 파일경로, 그릴크기W, H, 자를영역L, T, R, B, 오프셋X, Y)
ani->AddDrawImageInfo(L"Walk", 0.1f, L"player.png", 64, 64, 0, 0, 64, 64, 0, 0);
ani->AddDrawImageInfo(L"Walk", 0.1f, L"player.png", 64, 64, 64, 0, 128, 64, 0, 0);
```

#### 2. 애니메이션 재생 (`SetChangeAnimation`)

특정 이름의 애니메이션을 재생합니다. 반복 횟수(`LoopTime`)를 지정할 수 있습니다.

- `LoopTime`: -1이면 무한 반복, 1이면 한 번만 재생.

```cpp
// "Run" 애니메이션을 무한 반복
ani->SetChangeAnimation(L"Run", -1, 0);
```

#### 3. 업데이트 로직 (`Update`)

매 프레임 `Update(float dt)`가 호출되어 남은 시간(`m_RemineSec`)을 감소시킵니다. 시간이 0이 되면 다음 프레임으로 이미지를 교체합니다.

```cpp
void ImageAni_Component::Update(float dt) {
    // ... 시간 감소 ...
    if (m_RemineSec <= 0) {
        // 다음 프레임으로 인덱스 이동
        SetNextDatas(m_RemineSec);

        // 연결된 ImageComponent의 이미지와 영역 정보를 갱신
        SetUpdateImageComponent();
    }
}
```

## 사용법 (Usage)

1.  `GameObject`에 `ImageComponent`와 `ImageAni_Component`를 모두 추가합니다. (애니메이션 컴포넌트는 렌더링을 위해 `ImageComponent`가 필요합니다.)
2.  `AddDrawImageInfo`를 사용하여 필요한 애니메이션 프레임들을 등록합니다.
3.  `SetChangeAnimation`으로 원하는 동작을 시작합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "스프라이트 시트를 사용하는 것이 개별 이미지 파일을 여러 개 사용하는 것보다 성능상 어떤 이점이 있나요?"
- "`ImageAni_Component`가 직접 렌더링하지 않고 `ImageComponent`를 조작(Link)하는 방식을 사용한 이유는 무엇일까요? (컴포넌트의 역할 분리)"
- "프레임 간의 시간(`Duration`)을 다르게 설정하면 어떤 연출이 가능한가요? (예: 타격감을 위한 프레임 정지)"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`AddDrawImageInfo`를 일일이 코드로 치는 건 너무 힘들어. XML이나 JSON 파일에서 애니메이션 정보를 읽어와서 자동으로 등록하는 파서(Parser)를 만들어 줘."
- "공격 애니메이션이 끝난 후(`LoopTime`이 0이 되었을 때) 자동으로 대기(Idle) 애니메이션으로 돌아가는 상태 머신(State Machine) 로직을 구현해 줘."
- "특정 프레임에 도달했을 때 소리를 재생하거나 이펙트를 생성하는 '애니메이션 이벤트' 기능을 추가하고 싶어. 어떻게 구조를 잡아야 할까?"
