#pragma once
#include "Scene/Scene.h"


class PlantSelectCom;

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
	virtual ~Stage01( );

public:
	virtual void PrevInitSettings( ) override;
	virtual void InitSettings( ) override;

public:
	void InitSelectCom( ) override;
	void InitBGCom( ) override;
	void InitHeaderCard( ) override;



protected:
	PlantSelectCom* m_PlantSelectCom;
};

