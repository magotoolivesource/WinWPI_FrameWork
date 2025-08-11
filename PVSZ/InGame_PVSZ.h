#pragma once
#include <vector>


// https://chatgpt.com/c/6896c060-1ce8-8320-8fd2-82f29ea27a43
// 코드 추가방식
//#pragma comment(lib, "A_Framework.lib")

#include "Base_FrameWorkCls.h"
#include <Core/SingletonT.h>

using namespace std;


enum class E_STAGETYPE
{
	INGAME_LOBY,
	SELECT_PLANTS,
	STAGE01,
	STAGE02,

	MAX
};



class InGame_PVSZ : public Base_FrameWorkCls, public SingletonT< InGame_PVSZ>
{

public:
	Scene* GetCurrentScene( );

	//Component* AddCompoment( );
	GameObject* AddCurrentSceneGameObject(const std::string& name);

	static GameObject* AddGameObject(const std::string& name);


public:
	InGame_PVSZ( );
	virtual ~InGame_PVSZ( );
	virtual LRESULT InGameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	virtual void InitSettings( ) override;

protected:
	vector<Scene*> m_AllSceneVec;


protected:
	void InitStage01( );


public:
	void SetStageChangeType(E_STAGETYPE p_stagetype);
protected:
	E_STAGETYPE m_SelectStageType = E_STAGETYPE::MAX;



};
