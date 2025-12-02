# 26. 확장 코어 유틸리티 (Extended Core Utilities)

## 소개 (Introduction)

엔진의 안정성과 개발 편의성을 위해, 기본 기능 외에도 다양한 확장 유틸리티 클래스들이 제공됩니다.
문자열 변환, 수학 계산, 그리고 화면 렌더링 영역(Viewport)을 관리하는 헬퍼 클래스들을 소개합니다.

**관련 파일:**
- `FrameWork_2DAPI/Core/MyString.h`
- `FrameWork_2DAPI/Core/MyMath.h`
- `FrameWork_2DAPI/Core/Viewport.h`

## 1. 문자열 유틸리티 (MyString)

C++ 게임 개발에서는 `std::string` (ANSI, UTF-8)과 `std::wstring` (Unicode, Windows API) 사이의 변환이 빈번하게 발생합니다.
`MyString` 클래스는 이러한 인코딩 문제를 쉽게 해결해주는 정적 메서드를 제공합니다.

### 주요 기능

#### 문자열 포맷팅
`printf`처럼 형식 문자열을 사용할 수 있게 해줍니다.

```cpp
// "Score: 100" 문자열 생성
std::string msg = MyString::format_string("Score: %d", 100);
```

#### 인코딩 변환
Windows API 함수(`SetWindowTextW` 등)는 `wstring`을 요구하고, 파일 입출력이나 일반 로직은 `string`을 주로 사용하므로 변환이 필수적입니다.

- `WStringToString`: `wstring` -> `string` (UTF-8)
- `StringToWString`: `string` (UTF-8) -> `wstring`

```cpp
wstring name = L"플레이어";
string utf8Name = MyString::WStringToString(name);
```

## 2. 수학 유틸리티 (MyMath)

GDI+의 기하학적 계산을 도와주는 도구들입니다.

### 바운딩 박스 계산 (GetBoundBox)
회전된 물체의 정확한 영역(AABB)을 구하는 것은 충돌 처리나 렌더링 최적화에 매우 중요합니다.
`GetBoundBox`는 오브젝트의 변환 행렬(Transform Matrix)과 카메라 행렬을 적용하여, 화면상에서 차지하는 직사각형 영역을 계산해줍니다.

1.  로컬 좌표계의 4개 꼭짓점(0,0 ~ w,h)을 정의합니다.
2.  월드 행렬과 카메라 행렬을 곱해 4개 점을 변환합니다.
3.  변환된 점들의 최소/최대 x, y 값을 찾아 사각형을 만듭니다.

## 3. 뷰포트 (Viewport)

현재 카메라가 비추고 있는 화면 영역을 정의합니다.

- **`IsVisible`:** 특정 오브젝트가 화면 안에 있는지 검사합니다.
- **절두체 선별 (Frustum Culling):** 화면 밖의 오브젝트는 렌더링하지 않도록 최적화하는 데 사용됩니다. (현재 프레임워크에서는 `CameraManager`가 이 역할을 일부 수행하지만, `Viewport` 클래스를 통해 더 명시적으로 관리할 수 있습니다.)

```cpp
// 화면 안에 있을 때만 그리기
if (viewport.IsVisible(pos, width, height)) {
    object->Render(hdc);
}
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "`wstring`과 `string`의 차이는 무엇이며, 왜 Windows API는 `wstring`을 주로 사용하나요? (유니코드와 멀티바이트 문자 집합)"
- "`GetBoundBox`를 계산할 때 '회전된 사각형'을 다시 '축에 정렬된 사각형(AABB)'으로 만드는 이유는 무엇인가요? OBB(Oriented Bounding Box)와 비교했을 때 장단점은?"
- "`vsnprintf`를 사용하여 가변 인자(`...`)를 처리하는 원리에 대해 설명해 줘."

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`MyString` 클래스에 문자열을 특정 구분자로 자르는 `Split(string str, char delimiter)` 함수를 추가해 줘."
- "`MyMath`에 두 벡터 사이의 선형 보간(`Lerp`)뿐만 아니라, 구면 선형 보간(`Slerp`)을 구현하려면 어떻게 해야 할까?"
- "`Viewport` 클래스를 확장하여, 화면 좌표를 월드 좌표로 변환해주는 `ScreenToWorld(Vec2 screenPos)` 함수를 작성해 줘."
