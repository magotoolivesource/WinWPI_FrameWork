#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>
#include <functional>

#include <Core/SingletonT.h>


#include "StageInfo_WaveData.h"

class StageInfo_WaveElement;
class StageInfo_Wave;



class TimerIndexData
{
public:
	TimerIndexData() = default;
	virtual ~TimerIndexData( ) = default;

public:
	float m_ElapsedTime = 0.f;
	int m_CurrentIndex = -1; // 현재 인덱스
	StageInfo_WaveElement* m_CurrentData = nullptr; // 현재 웨이브 엘리먼트 데이터
};


class StageAdjustData
{
public:
	StageAdjustData() = default;
	virtual ~StageAdjustData( ) = default;

public:
	float m_ElapsedTime = 0.f;
	std::vector<StageInfo_WaveElement*> m_Waves;
};

class StageWaveManager : public SingletonT<StageWaveManager>
{
public:
	using Callback = std::function<void(StageInfo_WaveElement*, int)>;
	void SetCreateWaveZombi_CallFN(Callback p_fn)
	{
		m_CallBackFN = p_fn;
	}
protected:
	Callback m_CallBackFN = nullptr;

public:
	StageWaveManager();
	virtual ~StageWaveManager( ) override;


public:
	void Update(float dt);
	void ResetAllDatas( );
	void SetCurrentWaveIndex(int p_index);

	void Play( );
	void Pause( );

public:
	void SetStageInfoWaveData(StageInfoData* p_wavedata, Callback p_fn);
	StageInfo_WaveElement* GetCurrentWaveElementData( ) const;
	StageInfo_WaveElement* GetWaveElementDataAt( int p_index ) const;

protected:
	//void AdjustStageInfoData( );


public:
	float m_ElapsedTime = 0.f;
	int m_CurrentWaveIndex = -1;
	//vector< StageInfo_WaveElement* > m_AllWaveDataVec;

	bool m_ISLast = false;
	bool m_ISStart = false;

protected:
	StageInfoData* m_StageData = nullptr;
	StageInfo_WaveElement* m_CurrentWaveData;


	//vector<StageInfo_WaveElement*> m_AllWaveDataVec;
};

