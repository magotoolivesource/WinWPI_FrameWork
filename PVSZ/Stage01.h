#pragma once
#include "Scene/Scene.h"


class PlantSelectCom;
class StageInfoData;
class StageInfo_WaveElement;


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

public:
	void UpdateLoop(float dt) override;

protected:
	PlantSelectCom* m_PlantSelectCom;


public:
	void _Test_InitStageInfoData( );
protected:
	StageInfoData* m_StageInfoData = nullptr; // 스테이지 정보 데이터

protected:
	void CreateWaveZombi_CallFN(StageInfo_WaveElement* p_element, int p_index);

};

