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

	static void ClearAll()
	{
		//system("cls");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if ( hConsole == INVALID_HANDLE_VALUE ) return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);

		DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD count;
		COORD homeCoords = { 0, 0 };

		// 문자 지우기
		FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
		// 색상 속성 초기화
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
		// 커서 위치 초기화
		SetConsoleCursorPosition(hConsole, homeCoords);
	}

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

	static void Log(const std::string& msg, E_LogLevel level = E_LogLevel::E_INFO) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch ( level ) {
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
		std::cout << "[" << ( level == E_LogLevel::E_INFO ? "INFO" : level == E_LogLevel::E_WARN ? "WARN" : "ERROR" ) << "] " << msg << std::endl;

		SetConsoleTextAttribute(hConsole, 7); // 기본색으로 복원
	}
    
};
