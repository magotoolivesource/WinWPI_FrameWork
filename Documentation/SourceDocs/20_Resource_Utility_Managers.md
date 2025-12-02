# 20. 리소스 및 유틸리티 매니저 (Resource & Utility Managers)

## 소개 (Introduction)

게임이 커질수록 수많은 이미지 파일을 로드하고, 다양한 곳에서 공통적으로 사용되는 기능들이 필요해집니다.
이를 효율적으로 관리하기 위해 **리소스 매니저(ImageManager)**, **통합 접근자(FacadeManager)**, 그리고 **디버깅 도구(DebugObjectManager)**가 제공됩니다.

**관련 파일:**
- `FrameWork_2DAPI/Manager/ImageManager.h`
- `FrameWork_2DAPI/Manager/FacadeManager.h`
- `FrameWork_2DAPI/Manager/DebugObjectManager.h`

## 1. 이미지 매니저 (ImageManager)

동일한 이미지(예: "player.png")가 여러 오브젝트에서 사용될 때, 매번 파일에서 새로 로드하면 메모리가 낭비되고 속도가 느려집니다.
`ImageManager`는 이미지를 한 번만 로드하고, 이후 요청 시에는 저장된 이미지를 반환하는 **캐싱(Caching)** 기능을 수행합니다.

### 핵심 기능

- **`Load(path)`:**
    1. `std::unordered_map`을 확인하여 해당 경로의 이미지가 이미 있는지 검사합니다.
    2. 있다면 저장된 포인터를 반환합니다.
    3. 없다면 `Gdiplus::Image`로 새로 로드하여 맵에 저장 후 반환합니다.

```cpp
// 사용 예시
Gdiplus::Image* img = ImageManager::GetI()->Load(L"Assets/player.png");
```

## 2. 파사드 매니저 (FacadeManager)

**파사드 패턴(Facade Pattern)**을 응용하여, 게임 내의 여러 매니저(`Input`, `Timer`, `Scene` 등)를 통합 관리하고 초기화/해제 순서를 제어합니다.
개별 싱글톤(`GetI()`)에 접근하는 대신 `FacadeManager`를 통해 중앙에서 관리되는 구조를 지향할 수 있습니다.

### 핵심 기능
- **`Initlize()`:** 모든 매니저를 올바른 순서대로 초기화합니다.
- **`Release()`:** 모든 매니저의 메모리를 해제합니다.

## 3. 디버그 오브젝트 매니저 (DebugObjectManager)

개발 중에 눈에 보이지 않는 충돌 박스, 이동 경로, 변수 값 등을 화면에 그리기 위한 도구입니다.

### 핵심 기능

- **즉시 모드 그리기 (Immediate Mode Drawing):** `DrawLine`, `DrawRect` 등을 호출하면 해당 프레임에만 그려지고 다음 프레임에 사라집니다. (또는 수명을 지정할 수 있음)
- **풀링(Pooling):** 디버그용 선이나 텍스트 객체를 매번 `new/delete` 하지 않고, 미리 만들어둔 객체를 재사용하여 성능 저하를 막습니다 (`m_CacheLineSize` 등 활용).

```cpp
// 사용 예시: 적의 이동 경로를 빨간 선으로 표시
DebugObjectManager::GetI()->DrawLine(enemyPos, targetPos, 2.0f, Color::Red);
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`ImageManager` 없이 `new Gdiplus::Image(path)`를 직접 호출해서 사용하면, 몬스터가 100마리일 때 메모리에 어떤 일이 발생하나요?"
- "싱글톤 패턴(`SingletonT`)을 사용하는 매니저들을 `FacadeManager`로 묶어서 관리하면 어떤 장점이 있나요? (초기화 순서, 의존성 관리 등)"
- "디버그 렌더링에서 '오브젝트 풀링(Object Pooling)'을 사용하는 이유가 무엇인가요? 매 프레임 수백 개의 선을 그릴 때 성능에 어떤 영향을 미치나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`ImageManager`에 `Unload(path)` 또는 `ClearUnused()` 기능을 추가하여, 더 이상 쓰지 않는 이미지 메모리를 정리하는 코드를 작성해 줘."
- "`DebugObjectManager`를 이용하여 플레이어의 충돌 박스(AABB)를 초록색 사각형으로, 공격 범위(Circle)를 빨간색 원으로 그리는 디버깅 코드를 `Player::Render`에 추가해 줘."
- "사운드 파일(.wav, .mp3)을 관리하는 `SoundManager`를 `ImageManager`와 비슷한 구조로 만들어보고 싶어. `FMOD` 라이브러리를 사용한다고 가정하고 기본 뼈대를 짜 줘."
