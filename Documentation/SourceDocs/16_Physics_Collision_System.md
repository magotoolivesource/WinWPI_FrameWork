# 16. 물리와 충돌 시스템 (Physics & Collision System)

## 소개 (Introduction)

게임에서 물체가 움직이고 서로 부딪히는 것을 처리하기 위해, 이 프레임워크는 **물리(Physics)**와 **충돌(Collision)** 시스템을 분리하여 제공합니다.
- **물리:** 중력, 속도, 마찰력 등을 계산하여 물체의 위치를 이동시킵니다. (`PhysicsManager`, `RigidbodyComponent`)
- **충돌:** 물체끼리 겹치거나 닿았는지를 감지하고 반응합니다. (`CollisionManager`, `Collider`)

**관련 파일:**
- `FrameWork_2DAPI/Manager/PhysicsManager.h`, `PhysicsManager.cpp`
- `FrameWork_2DAPI/Manager/CollisionManager.h`, `CollisionManager.cpp`
- `FrameWork_2DAPI/Compoment/RigidbodyComponent.h`
- `FrameWork_2DAPI/Compoment/Collider.h`

## 1. 충돌 시스템 (Collision System)

물체 간의 겹침을 감지합니다. 효율적인 검사를 위해 **쿼드트리(Quadtree)** 공간 분할 알고리즘을 사용합니다.

### 핵심 개념

- **AABB (Axis-Aligned Bounding Box):** 회전하지 않은 직사각형 박스로 충돌을 단순화하여 계산합니다.
- **쿼드트리 (Quadtree):** 화면을 4분면으로 계속 쪼개어, 근처에 있는 물체들끼리만 충돌 검사를 하도록 최적화합니다.
- **충돌 이벤트:**
    - `OnCollisionEnter`: 처음 닿았을 때 한 번 호출
    - `OnCollisionStay`: 닿아 있는 동안 계속 호출
    - `OnCollisionExit`: 떨어지는 순간 한 번 호출

### 주요 기능

#### `UpdateCollisions()`
매 프레임 호출되어 모든 충돌 검사를 수행합니다.

1.  **쿼드트리 구성:** 현재 활성화된 모든 `Collider`를 쿼드트리에 넣습니다.
2.  **검사 후보 추출:** 각 `Collider`마다 근처에 있는(같은 노드에 속한) 다른 `Collider`들을 가져옵니다.
3.  **충돌 판정:** 두 `Collider`가 겹쳤는지 확인합니다. (`IntersectsWith` 등 사용)
4.  **이벤트 호출:** 이전 프레임의 충돌 상태와 비교하여 Enter, Stay, Exit 이벤트를 발생시킵니다.

#### `Raycast (레이캐스트)`
특정 지점에서 레이저(Ray)를 쏘아 물체가 닿는지 검사합니다. 총알 발사 판정이나 클릭 선택 등에 사용됩니다.

```cpp
PointF start(0, 0);
PointF end(100, 100);
PointF hitPoint;
Component* hitObj = CollisionManager::GetI()->Raycast(start, end, hitPoint);
if (hitObj) {
    // 무언가에 맞았다!
}
```

## 2. 물리 시스템 (Physics System)

물체에 물리 법칙을 적용하여 위치를 갱신합니다.

### 핵심 개념

- **Rigidbody (강체):** 물리 효과를 받는 컴포넌트입니다. 질량(Mass), 속도(Velocity), 가속도(Acceleration) 등을 가집니다.
- **중력 (Gravity):** `PhysicsManager`에 설정된 전역 중력 값(`Default: 0, -9.81f`)이 모든 리지드바디에 적용됩니다.
- **마찰력 (Friction/Drag):** 물체의 움직임을 서서히 멈추게 합니다.

### 주요 기능

#### `UpdatePhysics(float dt)`
매 프레임 호출되어 물체를 이동시킵니다.

```cpp
void PhysicsManager::UpdatePhysics(float dt) {
    for (auto& rb : m_PhysicsComVec) {
        // 1. 중력 적용
        rb->AddForce(Gravity * rb->GetMass());

        // 2. 속도에 따른 위치 변경 (p = p + v * dt)
        rb->owner->transform->position += rb->GetVelocity() * dt;

        // 3. 마찰력 적용 등...
    }
}
```

## 사용법 (Usage)

### 충돌체 추가하기

```cpp
// 1. 박스 콜라이더 추가 (위치, 크기 설정)
BoxCollider* col = obj->AddComponent<BoxCollider>(Vec2(0,0), 0, SizeF(50, 50));

// 2. 충돌 이벤트 받기 (람다 함수 사용 가능)
col->SetOnCollisionEnter([](Collider* other) {
    // 부딪혔을 때 실행될 코드
    printf("충돌 발생!");
});
```

### 물리 효과 적용하기

```cpp
// 1. 리지드바디 추가
RigidbodyComponent* rb = obj->AddComponent<RigidbodyComponent>();

// 2. 힘 가하기 (점프 예시)
if (InputManager::ISKeyDown(InputKey::Space)) {
    rb->AddForce(Vec2(0, 500.0f));
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`CollisionManager`에서 사용하는 '쿼드트리(Quadtree)'가 무엇이며, 이것을 사용하면 충돌 검사 성능이 얼마나 좋아지나요? (O(N^2) vs O(N log N))"
- "물리 엔진에서 'Enter', 'Stay', 'Exit'을 구분하는 알고리즘은 보통 어떻게 구현되나요? (Set 자료구조 사용 등)"
- "`Rigidbody` 없이 `Transform`의 위치를 직접 수정해서 움직이는 것과, `AddForce`나 `Velocity`를 사용하는 것의 차이는 무엇인가요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`Raycast`를 사용하여 마우스로 클릭한 위치에 있는 오브젝트를 선택하고, 그 오브젝트의 이름을 출력하는 코드를 작성해 줘."
- "플레이어가 바닥(`Platform` 태그)에 닿아있을 때만 점프할 수 있도록 하는 'IsGrounded' 체크 로직을 `OnCollisionStay`를 이용해 구현해 줘."
- "총알이 벽에 부딪히면 반사되는(Reflect) 기능을 만들고 싶어. 충돌 법선 벡터(Normal Vector)를 구해서 속도 벡터를 반사시키는 코드를 작성해 줘."
