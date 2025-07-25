﻿#pragma once
#include <windows.h>


class TimerManager {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER startTime;
    LARGE_INTEGER lastTime;
    double deltaTime;
    double totalTime;
    float fps = 0.f;

public:
    TimerManager() : deltaTime(0), totalTime(0) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&startTime);
        lastTime = startTime;
    }

    void Update() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        totalTime = static_cast<double>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
        lastTime = currentTime;

		if ( deltaTime > 0.f )
		{
            fps = 1.f / deltaTime;
		}
    }

    double GetDeltaTime() const {
        return deltaTime;
    }

    double GetTotalTime() const {
        return totalTime;
    }

	float GetFPS( )
	{
        return fps; // 1.f / deltaTime;
	}
};