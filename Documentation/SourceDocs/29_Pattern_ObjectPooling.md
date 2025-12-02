# 29. 디자인 패턴: 오브젝트 풀링 (Object Pooling)

## 소개 (Introduction)

총알(Bullet), 이펙트(Effect), 몬스터(Enemy) 등 게임 내에서 빈번하게 생성되고 파괴되는 오브젝트들은 `new`와 `delete`를 반복하며 심각한 성능 저하(메모리 파편화, 프레임 드랍)를 유발할 수 있습니다.
오브젝트 풀링은 미리 일정량의 오브젝트를 만들어두고(`Pool`), 필요할 때 꺼내 쓰고 다 쓰면 반납하는 방식으로 이를 해결합니다.

**관련 파일:**
- `PVSZ/ObjectPoolManager.h`, `ObjectPoolManager.cpp`

## 핵심 개념 (Core Concepts)

- **풀(Pool):** 사용 가능한 오브젝트들이 대기하고 있는 자료구조(주로 큐나 스택)입니다.
- **대여(Rent/Get):** 풀에서 오브젝트를 하나 꺼냅니다. 만약 풀이 비어있다면 새로 생성합니다. 꺼낸 오브젝트는 `SetActive(true)`로 활성화합니다.
- **반납(Return):** 사용이 끝난 오브젝트를 파괴하지 않고 `SetActive(false)`로 비활성화한 뒤 풀에 다시 넣습니다.

## 구현 분석 (Code Analysis)

`ObjectPoolManager`는 타입별로 풀을 관리하기 위해 `std::unordered_map`과 `std::type_index`를 사용합니다.

```cpp
// ObjectPoolManager.h (핵심 로직)
class ObjectPoolManager : public SingletonT<ObjectPoolManager> {
private:
    // 타입별로 큐(Queue)를 관리
    std::unordered_map<std::type_index, std::queue<std::shared_ptr<GameObject>>> pool;

public:
    template <typename T>
    std::shared_ptr<T> GetObject() {
        auto& q = pool[std::type_index(typeid(T))];

        if (!q.empty()) {
            // 풀에 있으면 꺼내서 재사용
            auto obj = std::static_pointer_cast<T>(q.front());
            q.pop();
            obj->SetActive(true);
            return obj;
        }

        // 없으면 새로 생성
        return std::make_shared<T>();
    }

    template <typename T>
    void ReturnObject(std::shared_ptr<T> obj) {
        // 비활성화 후 풀에 반납
        obj->SetActive(false);
        pool[std::type_index(typeid(T))].push(obj);
    }
};
```

## AI에게 할 수 있는 질문 (Questions for AI)

### 개념 이해
- "오브젝트 풀링을 사용하지 않고 `new/delete`를 반복하면 왜 '메모리 파편화(Fragmentation)'가 발생하나요?"
- "`std::vector` 대신 `std::queue`를 사용하여 오브젝트 풀을 구현한 이유는 무엇인가요?"
- "오브젝트를 재사용할 때 '초기화(Reset)'를 제대로 안 하면 어떤 버그가 발생할 수 있나요?"

### 기능 구현
- "총알이 화면 밖으로 나가면 자동으로 `ReturnObject`를 호출하여 풀로 반납하는 `AutoDestroy` 컴포넌트를 만들어 줘."
- "게임 시작 시(로딩 화면)에 좀비 오브젝트 50개를 미리 생성(`Pre-warm`)해서 풀에 채워넣는 코드를 작성해 줘."
