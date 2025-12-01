# Part 4. 게임플레이 메커니즘 (Gameplay Mechanics)

# 33. 게임플레이 메커니즘: 식물과 투사체 (Plants & Projectiles)

## 소개 (Introduction)

PVSZ 게임의 핵심 재미는 다양한 식물을 배치하고, 식물들이 좀비를 공격하거나 자원을 생산하는 상호작용에서 나옵니다.
이 문서에서는 식물(Plant), 투사체(Projectile), 그리고 자원(Resource) 오브젝트가 어떻게 구현되어 있는지 분석합니다.

**관련 파일:**
- `PVSZ/Plant_PeaShooter.h`, `Plant_PeaShooter.cpp` (식물)
- `PVSZ/Pea_Com.h`, `Pea_Com.cpp` (투사체)
- `PVSZ/Sun_Com.h`, `Sun_Com.cpp` (자원)

## 1. 식물: 완두콩 슈터 (`Plant_PeaShooter`)

완두콩 슈터는 일정 시간마다 전방으로 완두콩(Pea)을 발사하는 가장 기본적인 공격형 식물입니다.

### 공격 로직
`UtilTimerManager`를 사용하여 주기적으로 발사 함수를 호출합니다.

```cpp
void Plant_PeaShooter::Start() {
    // 3초마다 Call_CreatePeaFN 호출
    UtilTimerManager::GetI()->AddTimer(m_DelayShot,
        std::bind(&Plant_PeaShooter::Call_CreatePeaFN, this, ...),
        true); // 반복 실행
}

void Plant_PeaShooter::Call_CreatePeaFN(...) {
    // 1. 완두콩(Pea) 생성 (오브젝트 풀 사용 가능)
    Pea_Com* pea = Pea_Com::Create_PeaComObject();

    // 2. 공격력 등 데이터 전달
    pea->SetPlantATKData(&m_PlantATKData);

    // 3. 위치 설정 (식물 앞쪽)
    pea->transform->SetWorldPosition(...);
}
```

## 2. 투사체: 완두콩 (`Pea_Com`)

생성되면 오른쪽으로 계속 이동하며, 좀비와 충돌하면 데미지를 주고 사라집니다.

### 이동 및 충돌
- **이동:** `Update` 함수에서 `transform->position.x`를 증가시킵니다.
- **충돌:** `Collider_TriggerEnter` 이벤트에서 충돌 대상이 '좀비'인지 확인하고, 맞다면 데미지를 준 후 자신을 파괴(또는 풀 반납)합니다.

```cpp
void Pea_Com::Collider_TriggerEnter(Collider* src, Collider* dest, void* data) {
    // 상대방(dest)이 좀비인지 확인 (Tag 또는 Component 검사)
    auto* zombie = dest->owner->GetComponent<Base_ZombiActor>();
    if (zombie) {
        zombie->SetDamage(m_PlantATKData->Damage);
        // 자신(총알) 파괴
        this->Destroy();
    }
}
```

## 3. 자원: 태양 (`Sun_Com`)

플레이어가 식물을 심기 위해 필요한 자원입니다. 하늘에서 떨어지거나 해바라기가 생성합니다.

### 상호작용
마우스 클릭 이벤트를 감지하여, 클릭 시 자원을 증가시키고 오브젝트는 사라집니다.
`Button` 컴포넌트를 사용하거나, `InputManager`를 통해 직접 마우스 좌표와 AABB를 비교하여 구현할 수 있습니다.

```cpp
// Sun_Com::Update
if (InputManager::IsMouseLeftDown() && IsMouseOver()) {
    // 자원 증가 (GameManager 등 전역 관리자에 접근)
    InGame_PVSZ::GetI()->AddSun(50);

    // 이펙트 재생 후 삭제
    this->Destroy();
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "투사체(`Pea_Com`)가 이동할 때 `Transform`을 쓰는 것과 `Rigidbody`를 쓰는 것의 차이점은 무엇인가요? 유도탄을 만들려면 어느 쪽이 유리한가요?"
- "충돌 처리(`TriggerEnter`)에서 `dynamic_cast`를 사용하여 상대를 식별하는 방식의 성능 이슈와 대안(Tag, Layer, Component ID)에 대해 설명해 줘."

### 기능 구현
- "곡사포처럼 포물선을 그리며 날아가는 `MelonPult` 식물과 투사체를 만들고 싶어. 중력 가속도 공식을 적용한 이동 로직을 짜 줘."
- "폭탄(`CherryBomb`) 식물을 구현하고 싶어. 생성 후 1초 뒤에 주변 반경 100px 내의 모든 좀비에게 큰 데미지를 주고 사라지는 로직을 작성해 줘."
