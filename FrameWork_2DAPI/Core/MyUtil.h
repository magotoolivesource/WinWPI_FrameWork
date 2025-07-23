#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <format>
#include <ctime>

#include "../Core/Vector.h"
#include <cmath>

#include "SingletonT.h"


using namespace Gdiplus;

class MyUtil : SingletonT<MyUtil> 
{
public:
    MyUtil() = default;
    virtual ~MyUtil() = default;


	static int GetRandInt(int p_min = 0, int p_max = 100) 
	{
        static bool isinit = false;
        if (isinit == false) {
            std::srand(time(NULL));
            isinit = true;
        }

		int range = p_max - p_min + 1;
        int outrand = std::rand() % range + p_min;
        return outrand;
	}

	static float GetRandFloat(float p_min = 0.f, float p_max = 1.f) 
	{
        static bool isinit = false;
        if (isinit == false) {
            std::srand(time(NULL));
            isinit = true;
        }

        //int range = p_max - p_min + 1;
        //int outrand = std::rand() % range + p_min;

		float randomFloat = p_min + (static_cast<float>(rand()) / RAND_MAX) * (p_max - p_min);

        return randomFloat;
    }


	//// 중복되지 않는 랜덤 숫자 반환
 //   static int GetRandInt( int p_min, int p_max ) {

	//	static bool isinit = false;
	//	if ( isinit == false )
	//	{
 //           std::srand(time(NULL));
 //           isinit = true;
	//	}

 //       int range = p_max - p_min + 1;

 //       // 모든 숫자 사용 완료
 //       int usedCount = 0;
 //       for (bool u : used)
 //           if (u) ++usedCount;
 //       if (usedCount >= range) {
 //           throw std::runtime_error("모든 랜덤 숫자가 소진되었습니다.");
 //       }

 //       int val;
 //       do {
 //           val = std::rand() % range + minVal;
 //       } while (used[val - minVal]); // 이미 사용된 값이면 다시 생성

 //       used[val - minVal] = true;
 //       return val;
 //   }



};


//
//#include <iostream>
//#include <unordered_set>
//#include <random>
//#include <stdexcept>
////https://chatgpt.com/c/68808953-6924-8013-b4e6-e1ab650436a2
//class Random {
//private:
//    static inline std::mt19937 rng { std::random_device {}() }; // RNG
//    static inline std::unordered_set<int> used; // 이미 사용된 숫자
//    static inline int minVal = 0;
//    static inline int maxVal = 0;
//
//public:
//    // 초기화 (범위 설정)
//    static void Initialize(int min, int max) {
//        if (min > max) std::swap(min, max);
//        minVal = min;
//        maxVal = max;
//        used.clear();
//    }
//
//    // 중복되지 않는 랜덤 숫자 반환
//    static int GetInt() {
//        if ((int)used.size() >= (maxVal - minVal + 1)) {
//            throw std::runtime_error("모든 랜덤 숫자가 소진되었습니다.");
//        }
//
//        std::uniform_int_distribution<int> dist(minVal, maxVal);
//        int value;
//
//        do {
//            value = dist(rng);
//        } while (used.find(value) != used.end()); // 이미 사용된 값이면 다시 생성
//
//        used.insert(value);
//        return value;
//    }
//
//    // 남은 숫자 개수 반환
//    static size_t Remaining() { return (maxVal - minVal + 1) - used.size(); }
//};
//
