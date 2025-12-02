# Part 4. 게임플레이 메커니즘 (Gameplay Mechanics)

# 34. 스테이지 디자인 (Stage Design)

## 소개 (Introduction)

게임의 맵, 배경, 초기 배치, UI 등을 포함하는 '스테이지'를 어떻게 구성하는지 설명합니다.
`Stage01` 클래스는 `Scene`을 상속받아 실제 게임이 진행되는 하나의 레벨을 정의합니다.

**관련 파일:**
- `PVSZ/Stage01.h`, `Stage01.cpp`

## 1. 스테이지 초기화 (`InitSettings`)

씬이 로드될 때 호출되어 게임 환경을 구축합니다.

### 주요 단계
1.  **배경 설정 (`InitBGCom`):** 잔디밭 이미지를 배경으로 깔고, `BGImageCom` 컴포넌트를 추가합니다.
2.  **UI 초기화 (`InitHeaderCard`):** 상단의 식물 선택 패널(`UI_TopSelectPanelCom`)과 자원 표시 UI를 생성합니다.
3.  **그리드 설정 (`InitSelectCom`):** 식물을 배치할 수 있는 격자판(`PlantSelectCom`)을 설정합니다.
4.  **웨이브 데이터 로드:** 이 스테이지에 등장할 좀비 패턴(`StageInfoData`)을 설정하고 `StageWaveManager`에 등록합니다.

```cpp
void Stage01::InitSettings() {
    // 1. 배경 생성
    InitBGCom();

    // 2. 상단 UI 생성
    InitHeaderCard();

    // 3. 웨이브 데이터 설정
    _Test_InitStageInfoData();
    StageWaveManager::GetI()->SetStageInfoWaveData(m_StageInfoData, ...);
}
```

## 2. 웨이브 연동

스테이지는 `StageWaveManager`와 긴밀하게 작동합니다.
시간이 지나 좀비를 생성해야 할 때, 매니저는 스테이지에 등록된 **콜백 함수**(`CreateWaveZombi_CallFN`)를 호출합니다.

```cpp
void Stage01::CreateWaveZombi_CallFN(StageInfo_WaveElement* element, int index) {
    // 팩토리를 이용해 실제 좀비 생성
    auto* zombie = ZombieFactoryManager::GetI()->CreateZombie(element->m_ZombiName);

    // 생성 위치 설정 (화면 오른쪽 끝, 랜덤한 라인)
    float y = GetLineY(index); // 라인별 Y좌표 계산
    zombie->transform->SetWorldPosition(SCREEN_WIDTH + 50, y);
}
```

## 3. 리소스 관리

스테이지 종료 시(`Release`), 생성했던 배경, UI, 좀비, 식물 등 모든 오브젝트를 정리해야 합니다.
`Scene` 클래스의 기본 소멸자가 대부분 처리해주지만, 별도로 할당한 메모리(웨이브 데이터 등)가 있다면 여기서 해제합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "`Stage01`이 `Scene`을 상속받는 구조의 장점은 무엇인가요? `StageManager`를 따로 두는 것과 비교했을 때 어떤 차이가 있나요?"
- "하드코딩된 웨이브 데이터 대신 엑셀(CSV)이나 JSON 파일에서 스테이지 정보를 읽어오려면 어떻게 구조를 바꿔야 할까요?"

### 기능 구현
- "밤(Night) 스테이지인 `Stage02`를 만들고 싶어. 배경 이미지를 바꾸고, 하늘에서 떨어지는 태양(Sun)을 생성하지 않도록 로직을 변경해 줘."
- "스테이지 클리어 조건을 '모든 좀비 처치'에서 '제한 시간 버티기'로 바꾸려면 `StageWaveManager`와 `Stage01`을 어떻게 수정해야 할까?"
