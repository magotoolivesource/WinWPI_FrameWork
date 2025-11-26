# 7. ImageManager: 에셋 관리의 효율성

## 소개

`ImageManager`는 게임에 사용되는 이미지 에셋(Assets)을 효율적으로 로드하고 관리하는 싱글턴(Singleton) 클래스입니다. `ImageManager`의 가장 중요한 역할은 한 번 로드된 이미지를 메모리에 캐싱(Caching)하여 동일한 이미지가 여러 번 필요할 때 디스크에서 반복적으로 읽어오는 것을 방지하는 것입니다.

이 프레임워크에서는 이미지 에셋만 관리하지만, 사운드, 폰트 등 다른 종류의 에셋을 관리하는 `SoundManager`, `FontManager` 등을 유사한 방식으로 구현할 수 있습니다.

## 핵심 개념

### 이미지 캐싱(Image Caching)

게임에서 동일한 이미지가 여러 `GameObject`에 의해 사용되는 경우는 매우 흔합니다. 예를 들어, 수십 개의 동일한 적 캐릭터가 화면에 나타날 수 있습니다. `ImageManager`가 없다면 각 적 캐릭터가 생성될 때마다 디스크에서 적 이미지를 읽어와야 합니다. 이는 매우 느리고 비효율적인 작업입니다.

`ImageManager`는 이 문제를 다음과 같이 해결합니다.

1.  **최초 로드:** 특정 이미지(예: "enemy.png")가 처음 요청되면, `ImageManager`는 디스크에서 해당 파일을 읽어 `Gdiplus::Image` 객체를 생성합니다.
2.  **캐싱:** 생성된 `Image` 객체의 포인터를 이미지 파일의 경로("enemy.png")와 함께 내부 맵(Map)에 저장합니다.
3.  **재사용:** 이후 동일한 "enemy.png" 이미지가 다시 요청되면, `ImageManager`는 디스크를 읽는 대신 맵에 저장해 둔 `Image` 객체의 포인터를 즉시 반환합니다.

이러한 캐싱 메커니즘은 게임의 로딩 시간을 단축하고 성능을 향상시키는 데 매우 중요합니다.

### 메모리 관리

`ImageManager`는 소멸자(`~ImageManager()`)에서 캐싱된 모든 `Gdiplus::Image` 객체를 `delete`하여 메모리 누수를 방지합니다. `ImageManager`는 싱글턴이므로, 프로그램이 종료될 때 소멸자가 호출되면서 모든 이미지 리소스가 안전하게 해제됩니다.

## `ImageManager` 사용법

`ImageManager`는 `ImageComponent` 내부에서 자동으로 사용되므로, 일반적으로 개발자가 `ImageManager`를 직접 호출할 일은 많지 않습니다. 하지만 그 작동 방식을 이해하는 것은 중요합니다.

### `ImageComponent`를 통한 사용 (권장)

`ImageComponent`의 `ImageLoadImage` 메서드를 호출하면, 이 메서드가 내부적으로 `ImageManager`를 사용하여 이미지를 로드하고 캐싱합니다.

```cpp
// GameScene.cpp
#include "Core/GameObject.h"
#include "Compoment/ImageComponent.h"

void GameScene::InitSettings()
{
    // 적 1 생성
    GameObject* enemy1 = CreateObject("Enemy1");
    auto* renderer1 = enemy1->AddComponent<ImageComponent>();
    // "enemy.png"가 처음 요청되므로 디스크에서 로드되고 캐싱됩니다.
    renderer1->ImageLoadImage(L"Assets/enemy.png");

    // 적 2 생성
    GameObject* enemy2 = CreateObject("Enemy2");
    auto* renderer2 = enemy2->AddComponent<ImageComponent>();
    // "enemy.png"가 이미 캐싱되어 있으므로, 디스크를 읽지 않고 즉시 반환됩니다.
    renderer2->ImageLoadImage(L"Assets/enemy.png");
}
```

### 직접 사용 (필요 시)

만약 `ImageComponent`를 사용하지 않고 `Gdiplus::Image` 객체가 직접 필요한 경우가 있다면, `ImageManager`를 직접 호출할 수 있습니다.

```cpp
#include "Manager/ImageManager.h"

void CustomRender()
{
    Gdiplus::Image* myImage = ImageManager::GetI()->Load(L"Assets/custom_image.png");

    if (myImage != nullptr)
    {
        // myImage를 사용하여 GDI+ 렌더링 작업을 수행합니다.
        // ...
    }
}
```

## AI에게 할 수 있는 질문

### 개념 이해
- "디스크 I/O가 메모리 접근보다 훨씬 느리다고 하는데, `ImageManager`의 캐싱 기능이 게임 로딩 시간과 프레임 드랍(Frame Drop) 현상에 어떤 긍정적인 영향을 미치는지 구체적인 시나리오를 들어 설명해 줘. (예: 많은 적이 동시에 생성되는 상황)"
- "`ImageManager`가 싱글턴 패턴으로 구현된 이유는 무엇이며, 만약 싱글턴이 아니라면 어떤 문제가 발생할 수 있을지 설명해 줘. 예를 들어, `ImageManager` 인스턴스가 두 개 존재한다면?"
- "`ImageManager`의 소멸자에서 `delete pair.second;` 코드를 통해 메모리를 해제하는데, 만약 `Gdiplus::Image*` 대신 `std::shared_ptr<Gdiplus::Image>`를 사용하도록 `ImageManager`를 수정한다면 메모리 관리가 어떻게 달라지고 어떤 장단점이 있을까?"
- "현재 `ImageManager`는 `Load` 함수가 호출될 때 이미지를 로드해. 만약 게임 시작 시 필요한 모든 이미지를 미리 로드(Pre-loading)하고 로딩 진행률을 표시하는 기능을 구현하고 싶다면, `ImageManager`를 어떻게 확장해야 할지 설계 아이디어를 설명해 줘."

### 기능 구현 및 결과 도출
- "사운드 에셋을 관리하는 `SoundManager`를 `ImageManager`와 유사한 형태로 만들어보고 싶어. FMOD나 Wwise 같은 사운드 라이브러리를 사용한다고 가정하고, `SoundManager` 클래스의 전체적인 구조(`Load`, `Play`, `Stop` 함수 등)를 코드로 작성해 줘."
- "현재 `ImageManager`는 로드에 실패하면 `nullptr`을 반환해. 로드 실패 시 `nullptr` 대신 '이미지 없음(Image Not Found)'을 나타내는 기본 이미지를 반환하도록 `Load` 함수를 수정하는 코드를 보여줘."
- "`Scene`이 전환될 때, 이전 `Scene`에서는 사용했지만 새로운 `Scene`에서는 더 이상 필요 없는 이미지를 메모리에서 해제(Unload)하는 기능을 구현하고 싶어. 각 이미지가 몇 번이나 참조되고 있는지 계산(참조 카운팅, Reference Counting)하는 방식으로 `ImageManager`에 `Unload(const std::wstring& path)` 함수를 추가하는 코드를 작성해 줘."
- "게임 데이터를 JSON 파일로 저장하고 관리하는 `DataManager`를 만들어보고 싶어. `nlohmann/json` 라이브러리를 사용한다고 가정하고, JSON 파일을 읽어서 `std::map<std::string, MyData>` 형태로 캐싱하는 `DataManager`의 `LoadJson(const std::string& path)` 함수 구현 코드를 보여줘."
