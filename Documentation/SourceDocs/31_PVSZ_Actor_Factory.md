# Part 2. 게임 구조 분석 (Game Structure Analysis)

# 31. 액터와 팩토리 패턴 (Actor & Factory Pattern)

## 소개 (Introduction)

다양한 종류의 좀비(일반 좀비, 콘헤드 좀비 등)와 식물(완두콩 슈터, 해바라기 등)을 효율적으로 생성하고 관리하기 위해 **팩토리 패턴(Factory Pattern)**과 **상속 구조**를 사용합니다.

**관련 파일:**
- `PVSZ/Base_ZombiActor.h`
- `PVSZ/ZombieFactoryManager.h`

## 1. 액터 구조 (`Base_ZombiActor`)

모든 좀비의 부모 클래스입니다. 공통적인 속성과 행동을 정의합니다.

- **속성:** 체력(`HP`), 이동 속도(`Speed`), 공격력(`m_ZombiATKData`).
- **행동:**
    - `Update`: 이동 및 상태 확인.
    - `SetDamage`: 피격 처리 및 사망 체크.
    - `Collider_TriggerEnter`: 식물이나 총알과 충돌했을 때의 처리.

### 자동 등록 시스템 (Auto-Registration)
새로운 좀비 클래스를 만들 때마다 팩토리에 일일이 등록하는 번거로움을 줄이기 위해, **CRTP (Curiously Recurring Template Pattern)**와 정적 멤버 변수를 활용한 자동 등록 기법을 사용합니다.

```cpp
// Base_ZombiActor.h
template <typename TDerived>
class AutoZombie : public Base_ZombiActor {
protected:
    // 이 클래스가 생성될 때(프로그램 시작 시) 자동으로 팩토리에 등록됨
    static inline Registrar registrar;
};

// NormalZombie.h
class NormalZombie : public AutoZombie<NormalZombie> { ... };
```

## 2. 좀비 팩토리 (`ZombieFactoryManager`)

문자열(이름)이나 타입 정보를 기반으로 좀비 객체를 생성해주는 공장입니다.

### 핵심 기능
- **`RegisterZombie`:** 특정 타입의 좀비를 생성하는 함수(람다)를 맵(`creators`)에 저장합니다.
- **`CreateZombie`:** 요청된 타입의 생성 함수를 찾아 실행하고, 생성된 좀비 컴포넌트를 반환합니다.

```cpp
// 사용 예시
auto* zombie = ZombieFactoryManager::GetI()->CreateZombie<NormalZombie>();
```

이 방식을 사용하면 `StageWaveManager`는 구체적인 좀비 클래스(`NormalZombie`, `ConeHeadZombie` 등)를 알 필요 없이, 데이터에 적힌 대로 좀비를 찍어낼 수 있습니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "CRTP(Curiously Recurring Template Pattern)가 무엇이며, 여기서는 어떻게 '자동 등록'을 구현하는 데 사용되었나요?"
- "팩토리 패턴을 사용하면 `switch-case` 문으로 객체를 생성하는 것보다 어떤 점이 좋은가요? (OCP: 개방-폐쇄 원칙)"

### 기능 구현
- "`Base_ZombiActor`를 상속받아, 체력이 절반 이하로 떨어지면 이동 속도가 빨라지는 `AngryZombie`를 만들어 줘."
- "팩토리에서 좀비를 생성할 때, '오브젝트 풀(ObjectPoolManager)'을 우선적으로 확인하도록 `CreateZombie` 함수를 수정해 줘."
