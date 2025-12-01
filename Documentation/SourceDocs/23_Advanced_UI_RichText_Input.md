# 23. 고급 UI 시스템: RichText와 입력창 (Advanced UI: RichText & InputField)

## 소개 (Introduction)

기본적인 `TextComponent`만으로는 게임의 다양한 텍스트 표현(채팅, 데미지 표시, 사용자 입력 등)을 처리하기 어렵습니다.
`RichTextComponent`는 HTML 태그와 비슷한 방식으로 텍스트의 색상과 스타일을 문장 중간에 변경할 수 있게 해주며,
`InputFieldComponent`는 사용자의 키보드 입력을 받아 문자열을 저장하는 기능을 제공합니다.

**관련 파일:**
- `FrameWork_2DAPI/UICompoment/RichTextComponent.h`, `RichTextComponent.cpp`
- `FrameWork_2DAPI/UICompoment/RichTextAndInputFieldComponent.h`, `RichTextAndInputFieldComponent.cpp`
*(폴더명 오타 주의: `UICompoment` -> `UIComponent`)*

## 1. 리치 텍스트 (RichTextComponent)

문자열 안에 특정 태그를 포함시켜, 글자마다 다른 색상이나 스타일을 적용할 수 있는 고급 텍스트 컴포넌트입니다.

### 사용법

텍스트 문자열 안에 `<color=#AARRGGBB>내용</color>` 형식의 태그를 사용합니다.

```cpp
RichTextComponent* richText = obj->AddComponent<RichTextComponent>();
// "기본"은 검은색, "빨강"은 빨간색, "파랑"은 파란색으로 출력
richText->SetText(L"기본 텍스트 <color=#FFFF0000>빨강</color> 그리고 <color=#FF0000FF>파랑</color>");
```

### 내부 원리: 파싱 (Parsing)

`SetText`가 호출되면 내부적으로 `ParseRichText` 함수가 실행되어 문자열을 분석합니다.
1.  **정규표현식 (Regex):** `<color=...>(...)</color>` 패턴을 찾습니다.
2.  **세그먼트 분리:** 태그가 적용된 부분과 아닌 부분을 나누어 `TextSegment` 구조체로 저장합니다.
    - `TextSegment { wstring text; Color color; }`
3.  **렌더링:** `Draw` 함수에서 세그먼트들을 순회하며, 각기 다른 `Brush`(색상)를 사용하여 이어 그립니다.

## 2. 입력 필드 (InputFieldComponent)

사용자가 키보드로 글자를 입력할 수 있는 UI입니다. 채팅창, 아이디 입력, 수량 조절 등에 사용됩니다.

### 핵심 기능

- **포커스 (Focus):** 마우스 클릭 등으로 입력창이 활성화(`SetFocus(true)`)되었을 때만 입력을 받습니다.
- **문자 입력:** `InputManager`에서 `WM_CHAR` 메시지를 받아 문자를 추가합니다.
- **백스페이스:** 글자를 지우는 기능을 처리합니다.
- **콜백 (OnChange):** 텍스트가 변경될 때마다 등록된 함수를 호출하여 로직을 수행할 수 있습니다. (예: 실시간 검색)

```cpp
// 사용 예시
InputFieldComponent* input = obj->AddComponent<InputFieldComponent>();
input->SetPlaceholder(L"이름을 입력하세요...");
input->SetOnChange([](const std::wstring& text) {
    wcout << L"현재 입력: " << text << endl;
});
```

### 구현 상세

이 컴포넌트는 `InputManager`와 긴밀하게 협력해야 합니다. 특히 한글 입력(IME)을 처리하려면 윈도우 메시지(`WM_IME_COMPOSITION` 등)를 `InputManager`가 받아서 `InputFieldComponent`에 전달해주는 구조가 필요합니다.
(현재 예제 코드는 기본적인 영문/숫자 입력 위주로 구현되어 있습니다.)

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`RichText`에서 정규표현식(`std::wregex`)을 사용하는 것이 성능에 어떤 영향을 미칠까요? 매 프레임 파싱하는 것과 텍스트가 바뀔 때만 파싱하는 것의 차이는?"
- "게임에서 한글 입력(IME)을 처리하는 것이 왜 까다로운가요? '조합 중인 문자'와 '완성된 문자'의 차이를 설명해 줘."
- "`InputField`에 포커스가 있을 때 플레이어의 이동 키(W, A, S, D) 입력을 막으려면 `InputManager` 구조를 어떻게 변경해야 할까요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`RichText`에 `<b>`(볼드), `<i>`(이탤릭) 태그도 지원하도록 파싱 로직을 확장해 줘."
- "타이핑 게임을 만들고 싶어. `RichText`를 사용하여 플레이어가 입력해야 할 글자는 회색으로, 이미 입력한 글자는 초록색으로 표시하는 로직을 작성해 줘."
- "`InputField`에 커서(깜빡이는 막대) 기능을 추가하고 싶어. `TimerManager`를 이용하여 0.5초마다 커서를 그리고 지우는 코드를 구현해 줘."
