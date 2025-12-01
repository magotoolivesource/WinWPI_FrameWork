# 13. 핵심 아키텍처: GameObject와 Component (Core Architecture)

## 소개 (Introduction)

이 프레임워크는 **컴포넌트 기반 아키텍처 (Component-Based Architecture)**를 채택하고 있습니다.
이 설계 패턴에서 게임의 모든 물체는 `GameObject`라는 껍데기(Container)이며, 이 물체의 실제 동작과 데이터는 레고 블록처럼 조립 가능한 `Component`들이 담당합니다.

**관련 파일:**
- `FrameWork_2DAPI/Core/GameObject.h`, `GameObject.cpp`
- `FrameWork_2DAPI/Compoment/Component.h`, `Component.cpp`

## 핵심 개념 (Core Concepts)

- **상속보다 구성 (Composition over Inheritance):** 거대한 클래스 상속 계층(`Player` -> `Character` -> `Entity` ...)을 만드는 대신, 기능 단위로 쪼개진 컴포넌트를 조립하여 객체를 정의합니다. 유연성이 뛰어나고 코드 재사용성이 높습니다.
- **지연된 추가 및 제거 (Deferred Addition & Removal):** 게임 루프 도중 컴포넌트 리스트가 변경되어 발생할 수 있는 오류를 막기 위해, 변경 사항을 즉시 적용하지 않고 프레임 끝까지 대기했다가 안전하게 처리합니다.
- **소유권 (Ownership):** `GameObject`는 자신이 가진 `Component`들의 메모리 수명(생성부터 파괴까지)을 관리합니다.

## 주요 클래스 및 메서드 (Key Classes & Methods)

### `GameObject`

게임 월드에 존재하는 하나의 개체입니다.

#### 1. 컴포넌트 추가 (`AddComponent<T>`)

새로운 기능을 객체에 부여합니다.

```cpp
template<typename T, typename ...Args>
inline T* GameObject::AddComponent(Args && ...args)
{
    // 1. 컴포넌트 생성
    T* comp = new T(std::forward<Args>(args)...);

    // 2. 초기화 및 등록 (즉시 활성화되지 않고 대기열에 들어갈 수 있음)
    RegistComponent(comp);

    return comp;
}
```

#### 2. 컴포넌트 가져오기 (`GetComponent<T>`)

객체에 붙어있는 특정 기능을 찾아 반환합니다.

```cpp
template <typename T>
T* GetComponent() {
    // 1. 타입 정보를 이용해 해시맵에서 검색
    auto it = components.find(std::type_index(typeid(T)));

    // 2. 찾으면 반환, 없으면 nullptr
    if (it != components.end())
        return dynamic_cast<T*>(it->second.get());
    return nullptr;
}
```

#### 3. 생명주기 관리 (`Update`, `Render`)

자신이 가진 모든 컴포넌트의 생명주기 함수를 호출해 줍니다.

```cpp
void GameObject::Update(float dt) {
    // 1. 새로 추가된 컴포넌트가 있다면 초기화 (Start 호출)
    ProcessNewComponents();

    // 2. 활성화된 모든 컴포넌트 업데이트
    for (Component* comp : m_activeComponents) {
        if (comp->IsEnabled())
            comp->Update(dt);
    }
}
```

### `Component`

모든 기능의 부모 클래스입니다. 이를 상속받아 `SpriteRenderer`, `PlayerController` 등을 만듭니다.

- **`owner`:** 이 컴포넌트가 붙어있는 `GameObject`를 가리키는 포인터입니다.
- **`Start()`:** 컴포넌트가 생성된 후 첫 프레임 직전에 한 번 호출됩니다.
- **`Update(float dt)`:** 매 프레임 호출되어 로직을 수행합니다.
- **`Render(HDC hdc)`:** 화면에 무언가를 그릴 때 호출됩니다.

## 내부 작동 원리: 지연된 처리 (Deferred Processing)

`GameObject` 코드를 보면 `m_pendingComponents`(추가 대기열)와 `m_destroyQueue`(삭제 대기열)가 있습니다.

- **추가:** `AddComponent`를 호출하면 컴포넌트가 바로 `components` 리스트에 들어가는 것이 아니라, `ProcessNewComponents`가 호출될 때까지 대기합니다. 이를 통해 `Update` 루프 도중 리스트가 꼬이는 것을 방지합니다.
- **삭제:** `RemoveComponent`를 호출하면 해당 컴포넌트를 바로 `delete` 하지 않고, `m_destroyQueue`에 넣은 뒤 `m_ISObjectDirty` 플래그를 켭니다. 실제 메모리 해제는 안전한 시점에 일괄 처리됩니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`GameObject`에서 `AddComponent`를 할 때 템플릿(`template`)을 사용하는 이유는 무엇인가요? 인자를 전달하는 `std::forward`는 어떤 역할을 하나요?"
- "`std::unordered_map`과 `std::type_index`를 사용하여 컴포넌트를 관리하는 방식의 장점과 단점(성능 측면 등)은 무엇인가요?"
- "컴포넌트 패턴에서 객체 간의 통신(예: 플레이어가 적을 때렸을 때)은 보통 어떻게 구현하나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Component`를 상속받아 5초마다 체력을 회복하는 `RegenComponent`를 만들어 줘. `Update` 함수에서 `TimerManager`를 어떻게 활용해야 할까?"
- "`GameObject`에 특정 태그("Enemy")가 있는지 확인하고, 맞다면 데미지를 주는 기능을 구현해 줘."
- "`GetComponent`를 매 프레임 호출하는 것은 성능에 좋지 않다고 들었어. 이를 최적화하기 위해 `Start` 함수에서 미리 컴포넌트를 받아오는(Caching) 코드를 작성해 줘."
