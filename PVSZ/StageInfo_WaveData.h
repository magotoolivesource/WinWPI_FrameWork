#pragma once
#include <Compoment/Component.h>
#include <Compoment/ImageComponent.h>
#include <Compoment/RectLineComponent.h>
#include <Core/Vector.h>
#include <vector>

#include "InGameDefineDatas.h"

class UI_SelectBTNCom;
class Button;


using namespace std;

enum class E_LINETYPE
{
	LINE1 = 0,
	LINE2,
	LINE3,
	LINE4,
	LINE5,
	RANDOM,
	MAX,
};

class WaveLineData
{
public:
	WaveLineData(){}
	WaveLineData(E_LINETYPE p_linetype, E_ZOMBIE_TYPE p_zombitype) : E_LineType(p_linetype)
		, E_ZombieType(p_zombitype)
	{
	}
	virtual ~WaveLineData( ){}

	E_LINETYPE E_LineType = E_LINETYPE::MAX; // 라인 타입
	E_ZOMBIE_TYPE E_ZombieType = E_ZOMBIE_TYPE::MAX; // 좀비 타입

};

class StageInfo_WaveElement
{
public:
	StageInfo_WaveElement();
	virtual ~StageInfo_WaveElement( );

public:
	int m_WaveNumber = 0; // 현재 웨이브 번호
	int m_TotalWaveCount = 0; // 총 웨이브 수
	float m_WaveTime = 0.f; // 웨이브 시간
	float m_ElapsedTime = 0.f; // 경과 시간
	std::vector<WaveLineData*> m_ZombiDataVec;// m_PlantButtons; // 웨이브에 사용되는 식물 버튼들
};


class StageInfoData
{
public:
	//void AddWaveDuplicate(StageInfo_WaveElement* wave)
	//{
	//	bool isExist = false;
	//	for ( auto* item : m_Waves )
	//	{
	//		if ( item->m_ElapsedTime == wave->m_ElapsedTime )
	//		{
	//			isExist = true;
	//			for ( auto* element : wave->m_ZombiDataVec )
	//			{
	//				item->m_ZombiDataVec.push_back(element);
	//			}
	//		}
	//	}

	//	if ( !isExist )
	//	{
	//		m_Waves.push_back(wave);
	//	}
	//}

	void AddWave(StageInfo_WaveElement* wave, bool p_isduplicate = true)
	{
		if ( !p_isduplicate )
		{
			m_Waves.push_back(wave);
			return;
		}

		m_ISAdjustDupicate = true;
		bool isExist = false;
		for ( auto* item : m_Waves )
		{
			if ( item->m_ElapsedTime == wave->m_ElapsedTime )
			{
				isExist = true;
				for ( auto* element : wave->m_ZombiDataVec )
				{
					item->m_ZombiDataVec.push_back(element);
				}
			}
		}

		if ( !isExist )
		{
			m_Waves.push_back(wave);
		}
	}

	void AdjustWaveSorting( )
	{
		std::sort(m_Waves.begin(), m_Waves.end(), [](StageInfo_WaveElement* a, StageInfo_WaveElement* b) {
			return a->m_ElapsedTime < b->m_ElapsedTime;
			});

		AdjustDupicateDatas( );
	}

	const std::vector<StageInfo_WaveElement*>& GetWaves( ) const { return m_Waves; }
	StageInfo_WaveElement* GetCurrentWaveData(int p_index) const
	{
		if (p_index < 0 || p_index >= m_Waves.size())
			return nullptr;
		return m_Waves[ p_index ];
	}
	float GetCurrentWaveTime(int p_index) const
	{
		if (p_index < 0 || p_index >= m_Waves.size())
			return 0.f;
		return m_Waves[p_index]->m_WaveTime;
	}

protected:
	void AdjustDupicateDatas( )
	{
		StageInfo_WaveElement* tempwavedata = nullptr;
		auto iter = m_Waves.begin( );
		for ( ; iter != m_Waves.end( ); )
		{
			if ( tempwavedata != nullptr
				&& ( *iter )->m_ElapsedTime == tempwavedata->m_ElapsedTime )
			{
				for ( auto* element : ( *iter )->m_ZombiDataVec )
				{
					tempwavedata->m_ZombiDataVec.push_back(element);
				}

				iter = m_Waves.erase(iter);

				continue;
			}

			tempwavedata = *iter;
			++iter;
		}
	}

private:
	bool m_ISAdjustDupicate = false; // 중복 데이터 조정 여부
	std::vector<StageInfo_WaveElement*> m_Waves;
};


//class StageInfo_WaveSec
//{
//public:
//	float m_RemineSec = 0.f;
//	StageInfo_WaveElement* m_WaveData = nullptr;
//};





//class StageWaveData
//{
//public:
//	void Update(float dt);
//	void ResetAllDatas( );
//
//public:
//	float m_ElapsedTime = 0.f;
//	int m_CurrentWaveIndex = 0;
//	vector< StageInfo_WaveData* > m_AllWaveDataVec;
//};