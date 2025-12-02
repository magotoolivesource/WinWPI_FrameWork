# 27. 프로젝트 설정 및 빌드 환경 (Project Configuration & Build Environment)

## 소개 (Introduction)

C++ 프로젝트, 특히 Windows API를 사용하는 프로젝트는 코드가 실행되기 전에 **빌드 환경**과 **전처리기 설정**이 올바르게 되어 있어야 합니다.
이 문서는 `framework.h`, `targetver.h` 등의 헤더 파일들이 어떤 역할을 하는지, 그리고 프로젝트가 어떻게 구성되어 있는지 설명합니다.

**관련 파일:**
- `FrameWork_2DAPI/framework.h`
- `FrameWork_2DAPI/targetver.h`
- `FrameWork_2DAPI/Resource.h`

## 1. 미리 컴파일된 헤더 (Precompiled Header - PCH)

`framework.h`는 이 프로젝트의 **미리 컴파일된 헤더** 역할을 합니다.

### 역할
C++ 컴파일러는 `.cpp` 파일을 컴파일할 때마다 포함된 모든 헤더 파일을 처음부터 다시 읽고 처리합니다.
`windows.h`나 `vector`, `string` 같은 거대한 헤더 파일들을 매번 처리하면 빌드 속도가 매우 느려집니다.
PCH는 자주 변경되지 않는 무거운 헤더들을 미리 한 번만 컴파일해두고 재사용하여 빌드 시간을 획기적으로 단축시킵니다.

### 주요 내용 (`framework.h`)
- **`WIN32_LEAN_AND_MEAN`**: Windows 헤더에서 잘 쓰이지 않는 API들을 제외하여 컴파일 속도를 높입니다.
- **`windows.h`**: WinAPI 핵심 함수들을 포함합니다.
- **`gdiplus.h`**: 2D 그래픽 라이브러리입니다. (주의: `objidl.h`가 먼저 선언되어야 함)
- **표준 라이브러리:** `vector`, `map`, `string`, `algorithm` 등 자주 쓰는 STL을 여기에 몰아넣어 어디서든 사용할 수 있게 합니다.

```cpp
#pragma comment (lib,"Gdiplus.lib") // 링커에게 Gdiplus.lib를 링크하라고 지시
```

## 2. 타겟 버전 설정 (targetver.h)

`SDKDDKVer.h`를 포함하여, 이 프로그램이 실행될 수 있는 **최소 Windows 버전**을 정의합니다.
최신 Windows 기능을 사용하려면 여기서 버전을 높여야 하며, 호환성을 위해서는 낮춰야 합니다.

## 3. 리소스 관리 (Resource.h)

Windows 프로그램은 코드 외에도 아이콘, 커서, 메뉴, 대화상자 등의 **리소스**를 가집니다.
`Resource.h`는 이러한 리소스들에 고유한 **ID 번호**를 부여하는 매크로 파일입니다.

- `IDI_FRAMEWORK2DAPI`: 프로그램 아이콘 ID
- `IDC_MYICON`: 커서 ID
- `IDM_EXIT`: 메뉴의 '종료' 버튼 ID

`.rc` 파일(리소스 스크립트)이 이 헤더를 참고하여 리소스를 빌드된 실행 파일(.exe) 안에 포함시킵니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "미리 컴파일된 헤더(PCH)를 사용할 때 주의할 점은 무엇인가요? 자주 바뀌는 헤더를 PCH에 넣으면 어떤 일이 발생하나요?"
- "`WIN32_LEAN_AND_MEAN`을 정의하면 구체적으로 어떤 기능들이 빠지나요? 만약 소켓 프로그래밍(Winsock)을 하려는데 문제가 생긴다면 이걸 의심해봐야 할까요?"
- "`#pragma comment(lib, ...)`는 프로젝트 속성에서 라이브러리를 추가하는 것과 무엇이 다른가요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "내 프로젝트에 새로운 외부 라이브러리(예: `FMOD`)를 추가하고 싶어. 헤더 파일 경로와 라이브러리 경로를 Visual Studio 프로젝트 속성 어디에 설정해야 하는지 알려줘."
- "게임의 아이콘을 바꾸고 싶어. `Resource.h`와 `.rc` 파일을 수정해서 내가 만든 `.ico` 파일을 적용하는 방법을 단계별로 설명해 줘."
- "콘솔 창을 띄우지 않고 윈도우 창만 띄우려면 프로젝트의 '링커 - 시스템 - 하위 시스템' 설정을 어떻게 바꿔야 해?"
