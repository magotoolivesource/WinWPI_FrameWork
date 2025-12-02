# 21. 유틸리티와 트랜스폼 시스템 (Utilities & Transform System)

## 소개 (Introduction)

게임 프레임워크에는 여러 매니저 클래스들이 공통적으로 사용하는 패턴이나, 수학 계산을 도와주는 도구들이 필요합니다.
또한, 게임 오브젝트의 위치와 계층 구조를 관리하는 `Transform`은 엔진의 가장 기초적이면서도 중요한 부분입니다.

**관련 파일:**
- `FrameWork_2DAPI/Core/SingletonT.h`
- `FrameWork_2DAPI/Core/Vector.h`
- `FrameWork_2DAPI/Compoment/Transform.h`, `Transform.cpp`

## 1. 싱글톤 패턴 (SingletonT)

매니저 클래스(`TimerManager`, `InputManager` 등)가 프로그램 전체에서 **단 하나만 존재**하도록 보장하고, 어디서든 쉽게 접근할 수 있게 해주는 템플릿 클래스입니다.

```cpp
// 싱글톤 상속 예시
class MyManager : public SingletonT<MyManager> { ... };

// 사용 예시
MyManager::GetI()->DoSomething();
```

- **`GetI()`:** 인스턴스가 없으면 생성하고, 있으면 반환합니다.
- **자동/수동 소멸:** 프로그램 종료 시 자동으로 메모리를 해제하거나, 필요 시 `DestroyManager()`로 수동 해제할 수 있습니다.

## 2. 벡터 수학 (Vector.h)

2D 게임에서 위치, 속도, 방향 등을 표현하기 위한 `Vec2` 구조체입니다.

- **기본 연산:** 덧셈(`+`), 뺄셈(`-`), 스칼라 곱셈(`*`) 등이 연산자 오버로딩되어 있어 직관적으로 사용할 수 있습니다.
- **주요 함수:**
    - `Normalize()`: 벡터의 길이를 1로 만듭니다 (방향 벡터).
    - `Length()`: 거리(크기)를 구합니다.
    - `Dot()`: 내적을 구합니다.
    - `Rotate()`: 벡터를 회전시킵니다.

## 3. 트랜스폼 시스템 (Transform System)

모든 `GameObject`는 반드시 하나의 `Transform` 컴포넌트를 가집니다. 이는 물체의 위치(Position), 회전(Rotation), 크기(Scale)를 담당합니다.

### 계층 구조 (Parent-Child Hierarchy)
`Transform`은 다른 `Transform`을 부모(Parent)로 가질 수 있습니다.

- **지역 좌표 (Local Space):** 부모를 기준으로 한 상대적인 위치입니다.
- **월드 좌표 (World Space):** 게임 세상 전체를 기준으로 한 절대적인 위치입니다.

만약 부모가 이동하면, 자식들도 함께 이동합니다. (예: 자동차(`Parent`)가 움직이면 타고 있는 사람(`Child`)도 같이 이동)

### Dirty Flag 패턴
매 프레임 모든 오브젝트의 월드 좌표를 다시 계산하는 것은 비효율적입니다.
`Transform`은 값이 변경되었을 때만(`MarkDirty`), 월드 행렬을 다시 계산(`UpdateIfDirty`)하는 최적화 기법을 사용합니다.

```cpp
void Transform::SetLocalPosition(const Vec2& pos) {
    localposition = pos;
    MarkDirty(); // "나 변했어!"라고 표시
}

Vec2& Transform::GetWorldPosition() {
    UpdateIfDirty(); // 변한 게 있다면 재계산, 없다면 캐시된 값 반환
    return cachedWorldPosition;
}
```

### 피벗 (Pivot)
회전이나 크기 조절의 기준점입니다.
- 기본값은 (0, 0)이지만, 이미지의 중심을 기준으로 회전하고 싶다면 피벗을 조정해야 합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`Singleton` 패턴의 장점은 무엇이며, 너무 많이 사용하면(Global State) 어떤 문제가 생길 수 있나요?"
- "벡터의 `Normalize`가 필요한 이유는 무엇인가요? 대각선 이동 시 속도가 빨라지는 현상과 관련이 있나요?"
- "`MarkDirty`와 `UpdateIfDirty`를 사용하는 '지연된 계산(Lazy Evaluation)' 방식이 게임 성능에 어떤 도움을 주나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Transform`을 사용하여 태양을 공전하는 지구, 지구를 공전하는 달의 계층 구조를 코드로 작성해 줘."
- "플레이어가 마우스를 바라보도록 회전시키고 싶어. `atan2` 함수와 `Vector`를 사용하여 각도를 계산하는 코드를 알려줘."
- "두 `Vec2` 사이의 거리가 특정 값 이하일 때만 추적을 시작하는 AI 로직을 작성해 줘."
