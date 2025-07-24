#pragma once
#include <iostream>
#include <string>

#include "SingletonT.h"

enum class E_LogLevel 
{ 
	E_INFO, 
	E_WARN, 
	E_ERROR 
};

// 타이머 만들기
// https://chatgpt.com/c/6881e857-e234-8013-80d7-589d2cc13485

class UtilLoger {
public:
    static void Log(const std::wstring& msg, E_LogLevel level = E_LogLevel::E_INFO) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (level) {
        case E_LogLevel::E_INFO:
            SetConsoleTextAttribute(hConsole, 7);
            break; // 흰색
        case E_LogLevel::E_WARN:
            SetConsoleTextAttribute(hConsole, 14);
            break; // 노랑
        case E_LogLevel::E_ERROR:
            SetConsoleTextAttribute(hConsole, 12);
            break; // 빨강
        }
        std::wcout << "[" << (level == E_LogLevel::E_INFO ? "INFO" : level == E_LogLevel::E_WARN ? "WARN" : "ERROR") << "] " << msg << std::endl;

        SetConsoleTextAttribute(hConsole, 7); // 기본색으로 복원
    }
    
};
