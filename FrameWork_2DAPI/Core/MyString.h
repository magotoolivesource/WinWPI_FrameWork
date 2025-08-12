#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <format>

#include "../Core/Vector.h"
#include <cmath>



using namespace Gdiplus;

class MyString
{
public:
    MyString() = default;
    virtual ~MyString() = default;


	template <typename... Args> 
	static std::string format_string(const std::string& format, Args... args) {
        size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::unique_ptr<char[]> buffer(new char[size]);
        snprintf(buffer.get(), size, format.c_str(), args...);
        return std::string(buffer.get(), buffer.get() + size - 1);
    }
    //출처 : https : //wendys.tistory.com/116 [웬디의 기묘한 이야기:티스토리]

	static std::string format_string(const std::string fmt, ...) {


        int size = ((int)fmt.size()) * 2;
        std::string buffer;
        va_list ap;
        while (1) {
            buffer.resize(size);
            va_start(ap, fmt);
            int n = vsnprintf((char*)buffer.data(), size, fmt.c_str(), ap);
            va_end(ap);
            if (n > -1 && n < size) {
                buffer.resize(n);
                return buffer;
            }
            if (n > -1)
                size = n + 1;
            else
                size *= 2;
        }
        return buffer;
    }
    //출처 : https: //wendys.tistory.com/116 [웬디의 기묘한 이야기:티스토리]



	// https://chatgpt.com/c/6899b329-804c-8325-8c8a-02a65d63e9b4
	// wstring → string (UTF-8 기준)
	static std::string WStringToString(const std::wstring& wstr) {
		if ( wstr.empty( ) ) return {};

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0,
			wstr.c_str( ), ( int ) wstr.size( ),
			nullptr, 0, nullptr, nullptr);

		std::string str(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0,
			wstr.c_str( ), ( int ) wstr.size( ),
			&str[ 0 ], sizeNeeded, nullptr, nullptr);

		return str;
	}

	// string → wstring (UTF-8 기준)
	static std::wstring StringToWString(const std::string& str) {
		if ( str.empty( ) ) return {};

		int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0,
			str.c_str( ), ( int ) str.size( ),
			nullptr, 0);

		std::wstring wstr(sizeNeeded, 0);
		MultiByteToWideChar(CP_UTF8, 0,
			str.c_str( ), ( int ) str.size( ),
			&wstr[ 0 ], sizeNeeded);

		return wstr;
	}


	// ansi => UTF
	DWORD convert_ansi_to_unicode_string(__out std::wstring& unicode, __in const char* ansi, __in const size_t ansi_size) {
        DWORD error = 0;
        do {
            if ((nullptr == ansi) || (0 == ansi_size)) {
                error = ERROR_INVALID_PARAMETER;
                break;
            }
            unicode
                .clear(); 
			//        
			// // getting required cch.        
			// //         
			int required_cch = ::MultiByteToWideChar( CP_ACP,
				0,
				ansi, static_cast<int>(ansi_size),
				nullptr, 
				0 );
            if (0 == required_cch) {
                error = ::GetLastError();
                break;
            }
            unicode.resize( required_cch ); 

			//        
			// convert.        
			//         
			if (0 == ::MultiByteToWideChar(CP_ACP, 0, ansi, static_cast<int>(ansi_size), const_cast<wchar_t*>(unicode.c_str()),
                    static_cast<int>(unicode.size()))) 
			{
                error = ::GetLastError();
                break;
            }

        } while (false);


        return error;
    }
	//출처: https://wendys.tistory.com/84 [웬디의 기묘한 이야기:티스토리]


	// unicode => ansi
	DWORD convert_unicode_to_ansi_string(__out std::string& ansi, __in const wchar_t* unicode, __in const size_t unicode_size) {
        DWORD error = 0;
        do {
            if ((nullptr == unicode) || (0 == unicode_size)) {
                error = ERROR_INVALID_PARAMETER;
                break;
            }
            ansi.clear(); 
			//        
			// // getting required cch.        
			// //         
			int required_cch = ::WideCharToMultiByte(CP_ACP, 0, unicode, static_cast<int>(unicode_size), nullptr, 0, nullptr, nullptr);
            if (0 == required_cch) {
                error = ::GetLastError();
                break;
            } 

			//        
			// // allocate.        
			// //         
			ansi.resize( required_cch); 

			//        
			// // convert.        
			// //         
			if ( 0 == ::WideCharToMultiByte(CP_ACP, 0, unicode, static_cast<int>(unicode_size), const_cast<char*>(ansi.c_str()),
                    static_cast<int>(ansi.size()), nullptr, nullptr)) {
                error = ::GetLastError();
                break;
            }
        } while (false);

        return error;
    }
	//출처:        https: //wendys.tistory.com/84 [웬디의 기묘한 이야기:티스토리]





};

