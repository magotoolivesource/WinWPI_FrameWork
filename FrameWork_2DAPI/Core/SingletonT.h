﻿#pragma once
#include <windows.h>
#include <string>
#include <typeinfo> // typeid를 위해 필요

template <typename T>
class SingletonT
{
protected:
    SingletonT() = default;
    virtual ~SingletonT() = default;

    SingletonT(const SingletonT&) = delete;
    SingletonT& operator=(const SingletonT&) = delete;

private:
    static T* instance;

public:
    static T* GetI()
    {
        if (!instance)
        {
            instance = new T();
            static struct Deleter {
                ~Deleter() { 
					if ( instance )
					{
                        auto& id = typeid(instance);
						WCHAR wstr[256];
                        int result = MultiByteToWideChar(CP_ACP, 0, id.name(), -1, wstr, _countof(wstr));
                        wsprintf(wstr, L"%s : 매니저 자동소멸1\n", wstr);
                        ::OutputDebugString(wstr );

                        delete instance;
                        instance = nullptr; 
					}
				}
            } deleter;

			instance->CreateInitializeManager( );
        }
        return instance;
    }
    static T& Instance()
    {
        if (!instance)
        {
            instance = new T();
            static struct Deleter {
                ~Deleter() { 
					if (instance) {
                        auto& id = typeid(instance);
                        WCHAR wstr[256];
                        int result = MultiByteToWideChar(CP_ACP, 0, id.name(), -1, wstr, _countof(wstr));
                        wsprintf(wstr, L"%s : 매니저 자동소멸2\n", wstr);
                        ::OutputDebugString(wstr);

                        delete instance;
                        instance = nullptr;
                    }
                }
            } deleter;

			instance->CreateInitializeManager( );
        }
        return *instance;
    }

	virtual void DestroyManager()
	{
        if (instance) {
            auto& id = typeid( instance );

            WCHAR wstr[256];
            int result = ::MultiByteToWideChar(CP_ACP, 0, id.name(), -1, wstr, _countof(wstr));

            wsprintf(wstr, L"%s : 매니저 수동소멸\n", wstr);
            ::OutputDebugString(wstr);

            delete instance;
            instance = nullptr;
        }
	}

	virtual void Initialize()
	{
		// 초기화 작업
    }

	bool m_ISInit = false;
	virtual void CreateInitializeManager( )
	{
		// 초기화 작업
		if ( m_ISInit )
			return;

		m_ISInit = true;
	}
	//virtual void CreateInitialize( ) = 0;


};

template <typename T>
T* SingletonT<T>::instance = nullptr;
