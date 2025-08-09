#pragma once
#include "Scene/Scene.h"


class IStageInfo
{
public:
	virtual void InitSelectCom( ) = 0;
	virtual void InitBGCom( ) = 0;
	virtual void InitHeaderCard( ) = 0;
};



class Stage01 : public Scene, IStageInfo
{
public:
	virtual void PrevInitSettings( ) override;
	virtual void InitSettings( ) override;

public:
	void InitSelectCom( );
	void InitBGCom( );
	void InitHeaderUICard( );
};

