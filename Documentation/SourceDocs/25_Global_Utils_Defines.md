# 25. 전역 유틸리티와 매크로 (Global Utilities & Macros)

## 소개 (Introduction)

프로그래밍을 하다 보면 반복되는 코드를 줄이거나, 어디서든 편리하게 쓸 수 있는 수학 함수들이 필요해집니다.
이 프레임워크는 `DefineHeader.h`와 `MyUtil.h`를 통해 이러한 편의 기능들을 제공합니다.

**관련 파일:**
- `FrameWork_2DAPI/Core/DefineHeader.h`
- `FrameWork_2DAPI/Core/MyUtil.h`

## 1. 전역 매크로 (DefineHeader.h)

C++ 매크로(`define`)를 사용하여 코드를 간결하게 만듭니다.

### 메모리 해제 매크로
안전한 메모리 해제를 위해 `delete` 후 `nullptr` 대입을 한 번에 처리합니다.

- `SAFEDELETE(x)`: 단일 객체 삭제.
- `SAFEARRAYDELETE(x)`: 배열(`new []`) 삭제.
- `SAFEDELETEFOR(x)`: `std::vector` 등에 들어있는 포인터들을 모두 삭제하고 벡터를 비움.

### Getter/Setter 자동 생성
클래스의 멤버 변수에 대한 접근 함수를 자동으로 만들어줍니다. 반복적인 타이핑을 줄여줍니다.

```cpp
// 사용 예시
class Player {
    // int m_Hp 변수를 만들고, GetHp(), SetHp(val) 함수를 자동 생성
    GETSETER(int, Hp, 100);
};
```

### 수학 상수
- `PI`: 원주율 (3.14159...)
- `DEG2RAD`, `RAD2DEG`: 각도(Degree)와 라디안(Radian) 변환 상수.

## 2. 유틸리티 함수 (MyUtil.h)

`MyUtil` 클래스는 정적(static) 메서드들을 모아놓은 도구 상자입니다.

### 랜덤 함수 (Random Functions)
`rand()` 함수는 사용하기 불편하므로, 범위를 지정할 수 있는 래퍼 함수를 제공합니다.

- `GetRandInt(min, max)`: min ~ max 사이의 정수 난수를 반환합니다. (예: 주사위 1~6)
- `GetRandFloat(min, max)`: min ~ max 사이의 실수 난수를 반환합니다. (예: 0.0f ~ 1.0f 확률)

내부적으로 `srand(time(NULL))`을 최초 1회 자동 호출하여, 매번 다른 난수가 나오도록 초기화합니다.

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해 (Conceptual Understanding)
- "매크로(`define`)를 사용하는 것이 편리하긴 하지만, C++ 모던 코딩 스타일에서는 `template`이나 `inline function`을 더 권장하는 이유는 무엇인가요?"
- "`SAFEDELETE` 매크로에서 `if (x)` 검사를 하는 것이 성능에 영향을 미칠까요? `delete nullptr`는 안전한가요?"
- "`rand()` 함수는 '의사 난수(Pseudo Random)'라고 하는데, 보안이 중요한 기능(비밀번호 생성 등)에 사용해도 되나요? 게임에서는 왜 괜찮나요?"

### 기능 구현 및 결과 도출 (Implementation & Results)
- "`MyUtil` 클래스에 두 점 사이의 거리를 구하는 `GetDistance(p1, p2)` 함수와, 각도를 구하는 `GetAngle(p1, p2)` 함수를 추가해 줘."
- "`DefineHeader.h`에 싱글톤 패턴을 쉽게 선언할 수 있는 `DECLARE_SINGLETON(ClassName)` 매크로를 만들어 줘."
- "확률에 따라 아이템을 드랍하는 기능을 만들고 싶어. `MyUtil::GetRandFloat()`를 사용하여 5% 확률로 레어 아이템을 주는 로직을 작성해 줘."
