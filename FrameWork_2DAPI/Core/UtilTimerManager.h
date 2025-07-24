#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <chrono>
#include <thread>
#include <ctime>

#include "SingletonT.h"



// 타이머 만들기
// https://chatgpt.com/c/68819299-f13c-8013-8a68-ffc552ed6989

class UtilTimer {
public:
    using Callback = std::function<void( UtilTimer* )>;

    UtilTimer(float duration
		, Callback p_updatecallback
		, Callback callback
		, int p_repeatcount = 0  // -1이면 무한
		, bool updateEveryFrame = false)
        : duration(duration)
        , updatecallback(std::move(p_updatecallback))
        , completcallback(std::move(callback))
        , repeat(repeat)
        , updateEveryFrame(updateEveryFrame)
        , elapsed(0.0f)
        , active(true)
        , m_RemineSec (duration)
	{
        repeatCount = p_repeatcount;
		if ( p_repeatcount <= -1 )
		{
            repeat = true;
			//repeatcount 
		}
        else
		{
			repeat = false;
		}
            
	}

    void Update(float deltaTime) {
        if (!active) return;

        elapsed += deltaTime;
        m_RemineSec -= deltaTime;

        // 매 프레임마다 호출되는 업데이트 콜백
        if (updateEveryFrame && updatecallback) {
            updatecallback(this);
        }

        // 특정 시간이 지난 후 콜백
        if ( elapsed >= duration) {
            if (completcallback) completcallback(this);

			if (repeatCount <= -1) {
                elapsed = 0.0f; // 무한 반복
                m_RemineSec = duration;
            } else if (repeatCount > 0) {
                repeatCount--;
                elapsed = 0.0f;
                m_RemineSec = duration;
                if (repeatCount == 0) {
                    active = false;
                }
            } else { // repeatCount == 0
                active = false;
            }

            //if (repeat) {
            //    elapsed = 0.0f;
            //    m_RemineSec = duration;
            //} else {
            //    active = false; // 자동 비활성화
            //}
        }
    }

    void Stop() { active = false; }
    bool IsActive() const { return active; }
    float GetElapsed() const { return elapsed;  }
    float GetDuration() const { return duration; }
    float GetRemine() const { return m_RemineSec; }

private:
    float m_RemineSec;
    float duration;
    float elapsed;
    bool repeat;
    int repeatCount;
    bool updateEveryFrame;
    bool active;
    Callback completcallback;
    Callback updatecallback;

};

class UtilTimerManager : public SingletonT<UtilTimerManager> {
	using UtilTimerPtr = std::shared_ptr<UtilTimer>;

public:
    UtilTimerManager() = default;
    virtual ~UtilTimerManager() = default;

	int AddTimer(float duration, UtilTimer::Callback p_updatecallback, UtilTimer::Callback callback, int repeatcount = 0) {

        int id = ++lastId;
        auto timer = std::make_shared<UtilTimer>(duration, p_updatecallback, callback, repeatcount, true);
        timers.emplace_back(id, timer);
        return id;
    }

    int AddUpdateTimer(UtilTimer::Callback callback) {
        int id = ++lastId;
        auto timer = std::make_shared<UtilTimer>(0.0f, nullptr, callback, -1, true);
        timers.emplace_back(id, timer);
        return id;
    }

    void StopTimer(int id) {
        for (auto& [tid, timer] : timers) {
            if (tid == id) {
                timer->Stop();
                break;
            }
        }
    }

    void RemoveTimer(int id) {
        timers.erase( std::remove_if(timers.begin(), timers.end()
			, [id](auto& pair) { return pair.first == id; })
			, timers.end());
    }

    void Update(float deltaTime) {
        for (auto& [id, timer] : timers) {
            timer->Update(deltaTime);
        }

        // 완료된 타이머 제거
        timers.erase( std::remove_if(timers.begin()
				, timers.end()
				, [](auto& pair) { return !pair.second->IsActive(); } )
			, timers.end());
    }

private:
    std::vector<std::pair<int, UtilTimerPtr>> timers;
    int lastId = 0;


};

