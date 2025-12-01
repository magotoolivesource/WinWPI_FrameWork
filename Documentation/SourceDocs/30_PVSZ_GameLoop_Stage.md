# Part 2. 게임 구조 분석 (Game Structure Analysis)

# 30. 게임 루프와 스테이지 관리 (Game Loop & Stage Management)

## 소개 (Introduction)

PVSZ(Plants vs. Zombies 모작) 게임의 전체적인 흐름을 제어하는 구조에 대해 설명합니다.
`InGame_PVSZ` 클래스는 게임의 메인 진입점 역할을 하며, `StageWaveManager`는 좀비들이 몰려오는 '웨이브(Wave)' 시스템을 관리합니다.

**관련 파일:**
- `PVSZ/InGame_PVSZ.h`, `InGame_PVSZ.cpp`
- `PVSZ/StageWaveManager.h`, `StageWaveManager.cpp`

## 1. 메인 게임 루프 (`InGame_PVSZ`)

`Base_FrameWorkCls`를 상속받아 게임의 전체 생명주기를 관리하는 싱글톤 클래스입니다.

### 스테이지 관리
게임은 여러 장면(Scene)으로 구성됩니다.
- `E_STAGETYPE` 열거형을 사용하여 로비(`INGAME_LOBY`), 식물 선택(`SELECT_PLANTS`), 스테이지 1(`STAGE01`) 등으로 상태를 구분합니다.
- `InitStage01()`과 같은 함수에서 해당 스테이지에 필요한 배경, UI, 초기 오브젝트들을 생성합니다.

```cpp
void InGame_PVSZ::SetStageChangeType(E_STAGETYPE p_stagetype) {
    m_SelectStageType = p_stagetype;
    // 스테이지 전환 로직 (Scene 교체, 리소스 로드 등)
}
```

### 전역 오브젝트 관리
`AddGameObject`, `DestroyGameObject` 정적(static) 함수를 제공하여, 어디서든 현재 활성화된 씬에 오브젝트를 추가하거나 삭제할 수 있게 돕습니다.

## 2. 웨이브 매니저 (`StageWaveManager`)

디펜스 게임의 핵심인 "일정 시간마다 적들이 몰려오는 시스템"을 구현한 클래스입니다.

### 데이터 구조
- **`StageInfo_WaveElement`:** 하나의 웨이브에 대한 정보(등장할 좀비 종류, 수량, 시간 간격 등)를 담고 있습니다.
- **`StageInfoData`:** 전체 스테이지의 웨이브 리스트를 담고 있습니다.

### 웨이브 진행 로직
`Update(float dt)` 함수에서 시간이 흐름(`m_ElapsedTime`)에 따라 웨이브를 진행시킵니다.

1.  **시간 체크:** 현재 웨이브의 설정된 시간이 지났는지 확인합니다.
2.  **좀비 생성:** 시간이 되면 등록된 콜백 함수(`m_CallBackFN`)를 호출하여 좀비를 생성합니다. (`ZombieFactoryManager`와 연동)
3.  **다음 웨이브:** 모든 좀비가 생성되면 다음 웨이브 데이터로 넘어갑니다.

```cpp
void StageWaveManager::Update(float dt) {
    m_ElapsedTime += dt;
    // ... 시간 조건 충족 시 ...
    if (m_CallBackFN) {
        m_CallBackFN(m_CurrentWaveData, count); // 좀비 생성 요청
    }
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "`StageWaveManager`에서 콜백 함수(`std::function`)를 사용하여 좀비 생성 로직을 분리한 이유는 무엇인가요? (의존성 역전 원칙 등)"
- "게임의 상태(`E_STAGETYPE`)를 `enum`으로 관리할 때, 상태가 많아지면 어떻게 구조를 개선할 수 있을까요? (상태 패턴 등)"

### 기능 구현
- "`InGame_PVSZ`에 '일시 정지(Pause)' 기능을 추가하고 싶어. 모든 `Scene`과 `TimerManager`의 업데이트를 멈추는 코드를 작성해 줘."
- "웨이브가 끝날 때마다 'Wave Clear' UI를 띄우고, 5초 뒤에 다음 웨이브가 시작되도록 딜레이를 주는 로직을 구현해 줘."
