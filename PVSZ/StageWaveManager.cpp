#include "StageWaveManager.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>

#include <Core/InputManager.h>
#include <Core/GameObject.h>
#include <Compoment/ImageComponent.h>
#include <UICompoment/Button.h>
#include <Core/UtilLoger.h>

#include "InGameDefineDatas.h"
#include "InGame_PVSZ.h"
#include "UI_SelectBTNCom.h"


using namespace std;

StageWaveManager::StageWaveManager( )
{
}

StageWaveManager::~StageWaveManager( )
{

}

void StageWaveManager::Update(float dt)
{
	if ( !m_StageData ) return;

	if ( !m_ISStart )
		return;


	m_ElapsedTime += dt;

	int nextindex = m_CurrentWaveIndex + 1;
	StageInfo_WaveElement* waveelement = m_StageData->GetCurrentWaveData(nextindex);

	if ( waveelement == nullptr )
		return;

	if ( waveelement->m_ElapsedTime <= m_ElapsedTime )
	{
		m_CurrentWaveIndex = nextindex;
		m_CurrentWaveData = waveelement;

		// 이쪽에서 직접 오브젝트들 생성 할지 생각하기
		if ( m_CallBackFN )
		{
			m_CallBackFN(waveelement, m_CurrentWaveIndex);
		}
	}

}

void StageWaveManager::ResetAllDatas( )
{
	m_ElapsedTime = 0.f;
	m_CurrentWaveIndex = -1;
}

void StageWaveManager::SetCurrentWaveIndex(int p_index)
{
	m_CurrentWaveIndex = p_index;

	if ( m_StageData )
	{
		m_CurrentWaveData = m_StageData->GetCurrentWaveData(m_CurrentWaveIndex);
	}
	else
	{
		m_CurrentWaveData = nullptr;
	}
}

void StageWaveManager::Play( )
{
	m_ISStart = true;
}
void StageWaveManager::Pause( )
{
	m_ISStart = false;
}

void StageWaveManager::SetStageInfoWaveData(StageInfoData* stageData, Callback p_fn)
{
	m_StageData = stageData;
	m_CurrentWaveIndex = -1;
	m_ElapsedTime = 0.f;

	if( !m_StageData )
		m_StageData->AdjustWaveSorting( );

	m_CallBackFN = p_fn;
}

StageInfo_WaveElement* StageWaveManager::GetCurrentWaveElementData( ) const
{
	//int size = m_StageData->GetWaves( ).size( );
	//if(m_CurrentWaveIndex <= 0 || m_CurrentWaveIndex >= size)
	//{
	//	return nullptr; // 유효하지 않은 인덱스
	//}
	//return m_StageData->GetWaves( )[ m_CurrentWaveIndex ];

	return m_CurrentWaveData;
}

StageInfo_WaveElement* StageWaveManager::GetWaveElementDataAt(int p_index) const
{
	int size = m_StageData->GetWaves( ).size( );
	if ( p_index < 0 || p_index >= size )
	{
		return nullptr; // 유효하지 않은 인덱스
	}

	//return m_StageData->GetWaves( )[ p_index ];
	return m_StageData->GetCurrentWaveData(p_index);
}
