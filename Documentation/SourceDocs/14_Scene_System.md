# 14. 씬 시스템 (Scene System)

## 소개 (Introduction)

`Scene` 클래스는 게임의 특정 장면(스테이지, 메뉴, 타이틀 화면 등)을 관리하는 컨테이너입니다.
모든 `GameObject`는 특정 `Scene`에 소속되어 관리되며, 씬은 자신이 데리고 있는 모든 오브젝트의 업데이트와 렌더링, 생성과 파괴를 책임집니다.

**관련 파일:**
- `FrameWork_2DAPI/Scene/Scene.h`, `Scene.cpp`

## 핵심 개념 (Core Concepts)

- **오브젝트 관리자 (Object Manager):** 씬은 `std::vector<std::unique_ptr<GameObject>>`를 사용하여 오브젝트의 메모리를 소유하고 관리합니다.
- **렌더링 순서 정렬 (Z-Order Sorting):** 2D 게임에서는 겹치는 물체를 그릴 때 순서가 중요합니다. `Scene`은 오브젝트들의 깊이(`Depth`) 값을 기준으로 정렬하여 올바른 순서대로 그립니다.
- **지연된 생성과 파괴 (Deferred Creation & Destruction):** `GameObject`와 마찬가지로, 게임 루프 도중 리스트가 변경되는 것을 막기 위해 오브젝트의 생성과 삭제 요청을 모아두었다가 안전한 시점에 처리합니다.

## 주요 메서드 (Key Methods)

### `CreateObject(const std::string& name)`

새로운 게임 오브젝트를 생성하고 씬에 등록합니다.

```cpp
GameObject* Scene::CreateObject(const std::string& name)
{
    // 1. 유니크 포인터로 오브젝트 생성
    auto obj = std::make_unique<GameObject>(this);
    obj->SetName(name);

    GameObject* ptr = obj.get();

    // 2. 대기열에 추가 (즉시 활성화되지 않음)
    m_pendingStartObjects.push_back(ptr);         // Start() 호출 대기열
    m_pendingObjects.emplace_back(std::move(obj)); // 메모리 소유권 대기열

    // 3. 정렬 필요 플래그 설정
    isDirty = true;

    return ptr;
}
```

### `UpdateLoop(float dt)`

매 프레임 호출되어 씬 내부의 상태를 갱신합니다.

1.  **새로운 오브젝트 처리 (`ProcessNewObjects`):** 방금 생성된 오브젝트들의 `Start()`를 호출하고, 실제 업데이트 리스트(`m_sortedObjects`)로 옮깁니다.
2.  **런타임 컴포넌트 처리 (`ProcessRunTimeAddComponentObject`):** 실행 중에 오브젝트에 붙은 새로운 컴포넌트들을 초기화합니다.
3.  **정렬 (`UpdateIfDirty`):** 만약 `isDirty` 플래그가 켜져 있다면, 오브젝트들을 깊이(Depth) 순서로 다시 정렬합니다.
4.  **오브젝트 업데이트:** 정렬된 리스트를 순회하며 모든 오브젝트의 `Update(dt)`를 호출합니다.

```cpp
void Scene::UpdateLoop(float dt)
{
    // 1. 대기 중인 오브젝트/컴포넌트 실체화
    ProcessNewObjects();
    ProcessRunTimeAddComponentObject();

    // 2. 그리기 순서 정렬 (필요한 경우에만)
    UpdateIfDirty();

    // 3. 모든 오브젝트 업데이트
    for (size_t i = 0; i < m_sortedObjects.size(); i++)
    {
        m_sortedObjects[i]->Update(dt);
    }
}
```

### `Render(HDC p_hdc)`

정렬된 순서대로 오브젝트들을 그립니다.

```cpp
void Scene::Render(HDC p_hdc)
{
    // 정렬된 순서대로 그리기 (뒤에 있는 것이 먼저 그려짐)
    for (auto& obj : m_sortedObjects)
        obj->Render(p_hdc);
}
```

### `DestroyObject(GameObject* obj)`

오브젝트를 삭제합니다. 즉시 지우지 않고 대기열에 넣습니다.

```cpp
void Scene::DestroyObject(GameObject* obj) {
    // 1. 자식 오브젝트들도 재귀적으로 삭제 요청
    auto& children = obj->transform->GetChildren();
    for (auto* childTransform : children) {
        if (childTransform && childTransform->owner) {
            DestroyObject(childTransform->owner);
        }
    }

    // 2. 삭제 대기열에 추가 (중복 방지)
    if (std::find(m_destroyQueue.begin(), m_destroyQueue.end(), obj) == m_destroyQueue.end()) {
        m_destroyQueue.push_back(obj);
    }
}
```

## 심화 분석: `ProcessDestroyQueue`

`BaseGame::Render`가 끝난 직후 호출되며, 이때 실제 메모리 해제가 일어납니다.

1.  `m_destroyQueue`에 있는 오브젝트들을 `m_sortedObjects`(업데이트/렌더링용 리스트)에서 제거합니다.
2.  `m_AllOjects`(메모리 소유용 벡터)에서 제거하여 `unique_ptr`을 파괴하고 메모리를 반환합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`std::vector<std::unique_ptr<GameObject>>`를 사용하여 오브젝트를 관리하는 것의 장점은 무엇인가요? 메모리 관리 측면에서 설명해 줘."
- "`ProcessNewObjects`를 `UpdateLoop`의 시작 부분에서 호출하는 이유는 무엇인가요? 만약 중간이나 끝에서 호출하면 어떤 문제가 생길 수 있나요?"
- "게임에서 'Z-Order' 또는 'Depth' 정렬이 왜 필요한가요? `UpdateIfDirty` 함수가 하는 역할을 설명해 줘."

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Scene` 클래스를 상속받아 `TitleScene`을 만들고, 스페이스 바를 누르면 `GameScene`으로 전환되는 로직을 구현하고 싶어. `SceneManager`가 필요할까?"
- "특정 이름을 가진 오브젝트를 찾는 `GameObject* FindObject(string name)` 함수를 `Scene` 클래스에 추가해 줘."
- "모든 몬스터 오브젝트를 한 번에 죽이는(삭제하는) 기능을 만들고 싶어. `Tag` 기능을 이용해서 'Monster' 태그가 달린 모든 오브젝트를 `DestroyObject` 하는 함수를 작성해 줘."
