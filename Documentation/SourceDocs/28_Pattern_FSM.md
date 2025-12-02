# 28. 디자인 패턴: 유한 상태 머신 (FSM: Finite State Machine)

## 소개 (Introduction)

게임 캐릭터는 대기(Idle), 이동(Move), 공격(Attack), 사망(Die) 등 다양한 **상태(State)**를 가집니다.
FSM은 이러한 상태들을 명확히 구분하고, 특정 조건에 따라 상태를 전환하는 구조를 제공하여 복잡한 행동 로직을 관리하기 쉽게 만듭니다.

**관련 파일:**
- `PVSZ/Actor_State_Com.h`, `Actor_State_Com.cpp`
- `PVSZ/Base_ZombiActor.h` (실제 활용 예시)

## 핵심 개념 (Core Concepts)

### 상태 (State)
캐릭터가 취할 수 있는 행동의 단위입니다. 각 상태는 다음과 같은 메서드들을 가질 수 있습니다.
- **Enter:** 상태에 처음 진입할 때 실행 (예: 이동 애니메이션 시작).
- **Update:** 해당 상태일 때 매 프레임 실행 (예: 좌표 이동).
- **Exit:** 다른 상태로 전환될 때 실행 (예: 이동 애니메이션 종료).

### 상태 전환 (Transition)
어떤 조건이 만족되었을 때 상태를 바꾸는 것입니다.
- 예: "HP가 0 이하가 되면" -> `Die` 상태로 전환.
- 예: "적과의 거리가 100 이하가 되면" -> `Attack` 상태로 전환.

## 구현 분석 (Code Analysis)

`PVSZ` 예제에서는 `Actor_State_Com`을 통해 기본적인 체력 관리와 사망 상태 처리를 구현하고 있습니다.

```cpp
// Actor_State_Com.h (간소화)
class Actor_State_Com : public Component {
protected:
    int HP;
    bool ISDie = false; // 사망 상태 플래그

public:
    void SetDamage(float damage) {
        HP -= damage;
        if (HP <= 0) {
            ISDie = true;
            // 여기서 사망 애니메이션 재생 등의 상태 전환 로직 추가 가능
        }
    }
};
```

더 발전된 형태로는 `enum class State { Idle, Move, Attack }`를 사용하고, `Update` 함수 내에서 `switch-case` 문으로 분기 처리하거나, `State` 클래스를 별도로 만들어 관리하는 방식이 있습니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "FSM(유한 상태 머신)과 BT(행동 트리, Behavior Tree)의 차이점은 무엇이며, 어떤 상황에서 FSM을 쓰는 게 더 유리한가요?"
- "`switch-case` 문을 이용한 FSM 구현과 `State` 패턴(클래스 상속)을 이용한 구현의 장단점을 비교해 줘."

### 기능 구현
- "`Actor_State_Com`을 확장해서 `Idle`, `Move`, `Attack` 3가지 상태를 관리하는 `EnemyFSM` 컴포넌트를 코드로 짜 줘. `enum class`를 사용해서."
- "상태가 바뀔 때마다(`EnterState`) 이벤트를 발생시켜서, 애니메이션을 자동으로 교체해주는 코드를 작성해 줘."
