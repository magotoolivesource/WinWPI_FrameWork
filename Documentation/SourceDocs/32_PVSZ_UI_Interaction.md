# Part 3. UI 및 상호작용 (UI & Interaction)

# 32. 인게임 UI와 상호작용 (Ingame UI & Interaction)

## 소개 (Introduction)

PVSZ 게임에서 플레이어는 상단 패널의 식물 카드를 선택하고, 게임 맵(그리드)에 배치하여 좀비를 막습니다.
이 과정에서 UI 클릭, 자원(Sun) 확인, 그리드 스냅(Snap) 등의 상호작용이 일어납니다.

**관련 파일:**
- `PVSZ/UI_TopSelectPanelCom.h`, `UI_TopSelectPanelCom.cpp`
- `PVSZ/PlantSelectCom.h`, `PlantSelectCom.cpp`

## 1. 식물 선택 패널 (`UI_TopSelectPanelCom`)

화면 상단에 위치하며, 플레이어가 사용할 수 있는 식물 카드들을 보여줍니다.

### 주요 기능
- **카드 목록 관리:** `m_SelectBTNComVec` 벡터에 각 식물에 해당하는 버튼 컴포넌트(`UI_SelectBTNCom`)들을 저장합니다.
- **클릭 처리:** 버튼이 클릭되면 `Click_CallBackFN`이 호출되어, 현재 선택된 식물 타입(`m_PlantType`)을 저장합니다.

```cpp
void UI_TopSelectPanelCom::Click_CallBackFN(Button* btn, int selectID) {
    // 1. 선택된 식물 타입 저장
    m_PlantType = (E_PlantType)selectID;

    // 2. 이전에 선택된 버튼의 강조 효과 끄기
    if (m_CurrentSelectBTN) m_CurrentSelectBTN->SetHighlight(false);

    // 3. 새로 선택된 버튼 강조
    m_CurrentSelectBTN = btn;
}
```

## 2. 식물 배치 시스템 (`PlantSelectCom`)

식물을 선택한 상태에서 마우스를 맵 위로 가져가면, 식물을 심을 위치를 미리 보여주는(Ghost Image) 역할을 합니다.

### 그리드 스냅 (Grid Snap)
식물은 아무 데나 심을 수 없고, 정해진 타일(80x98 크기)에 맞춰 심어야 합니다.
마우스 좌표를 타일 크기로 나누어 인덱스를 구하고, 다시 좌표로 변환하여 딱 맞는 위치를 계산합니다.

```cpp
// 그리드 인덱스 계산
int gridX = (int)(mousePos.x / BLOCK_WIDTH);
int gridY = (int)(mousePos.y / BLOCK_HEIGHT);

// 스냅된 위치 계산
float snapX = gridX * BLOCK_WIDTH;
float snapY = gridY * BLOCK_HEIGHT;
```

### 배치 로직
마우스 왼쪽 버튼을 클릭하면, 현재 선택된 식물이 있고 자원이 충분한지 확인한 후 실제로 식물 오브젝트를 생성합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "그리드 스냅(Grid Snap)을 구현할 때 정수 나눗셈을 이용하는 원리가 무엇인가요?"
- "UI 컴포넌트(`UI_TopSelectPanelCom`)와 게임 로직(`PlantSelectCom`)이 서로 통신할 때, 결합도(Coupling)를 낮추기 위해 어떤 패턴을 쓸 수 있을까요? (옵저버 패턴, 이벤트 버스 등)"

### 기능 구현
- "식물을 심을 때 필요한 자원(Sun)이 부족하면 카드를 회색으로 비활성화(Gray-scale)하고 클릭되지 않게 하는 기능을 추가해 줘."
- "식물을 잘못 심었을 때 취소할 수 있도록, 마우스 오른쪽 클릭 시 선택을 해제하는 코드를 작성해 줘."
