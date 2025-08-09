#pragma once
#include <vector>


// https://chatgpt.com/c/6896c060-1ce8-8320-8fd2-82f29ea27a43
// 코드 추가방식
//#pragma comment(lib, "A_Framework.lib")

#include "Base_FrameWorkCls.h"

using namespace std;


class InGame_PVSZ : public Base_FrameWorkCls
{
public:
	InGame_PVSZ( );
	virtual ~InGame_PVSZ( );
	virtual LRESULT InGameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	virtual void InitSettings( ) override;

protected:
	vector<Scene*> m_AllSceneVec;
};
