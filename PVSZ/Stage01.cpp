#include "Stage01.h"
#include <functional>
#include <fcntl.h>
#include <Core/MyUtil.h>

#include <Core/Vector.h>
#include "Core/GameObject.h"
#include "Compoment/Transform.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "PlantSelectCom.h"
#include "UI_TopSelectPanelCom.h"
#include "PlantSelectCom.h"

#include "StageInfo_WaveData.h"
#include "StageWaveManager.h"
#include "InGameDefineDatas.h"

#include "ZombieFactoryManager.h"
#include "Base_ZombiActor.h"
#include "NormalZombie.h"


Stage01::~Stage01( )
{
	SAFEDELETE(m_StageInfoData);
}

void Stage01::PrevInitSettings()
{
	__super::PrevInitSettings();
}

void Stage01::InitSettings()
{
	__super::InitSettings();

	_Test_InitStageInfoData( );

	// 스테이지 정보 등록
	//StageWaveManager::GetI( )->SetStageInfoWaveData(nullptr);

	InitBGCom( );
	InitSelectCom();
	InitHeaderCard();
}

void Stage01::InitSelectCom()
{
	Gdiplus::RectF rectF(100, 100, 200, 100);
	GameObject* linerect = this->CreateObject("linerect");
	PlantSelectCom* rectlinecom = linerect->AddComponent<PlantSelectCom>();
	rectlinecom->SetIsTransformMatrix(true);
	

	m_PlantSelectCom = rectlinecom;

}

void Stage01::InitBGCom()
{
	// 배경 그리기
	auto gbobj = this->CreateObject("BG");
	auto bgcom = gbobj->AddComponent<ImageComponent>(nullptr, 0, 0, true);
	bgcom->ImageLoadImage(L"Assets/Images/Background/background1.jpg");
	bgcom->ResetUseDrawRect( );
	//bgcom->SetDrawRect()
	bgcom->transform->SetWorldPosition(0, 0);
	bgcom->transform->SetDepth(-100);
}

void Stage01::InitHeaderCard()
{
	auto uipanel = this->CreateObject("판넬");

	UI_TopSelectPanelCom* topuipanelcom = uipanel->AddComponent<UI_TopSelectPanelCom>( );
	topuipanelcom->transform->SetDepth(10);
	topuipanelcom->InitSettings( );
	

	// 기본 등록 하기용
	m_PlantSelectCom->m_TopSelectPanelCom = topuipanelcom;
}

void Stage01::UpdateLoop(float dt)
{
	StageWaveManager::GetI( )->Update(dt);

	__super::UpdateLoop(dt);
	
}

void Stage01::_Test_InitStageInfoData( )
{

	m_StageInfoData = new StageInfoData( );

	// 10초 생성
	StageInfo_WaveElement* wavedata = nullptr;

	wavedata = new StageInfo_WaveElement( );
	wavedata->m_ElapsedTime = 3.f;
	wavedata->m_ZombiDataVec.push_back(
		new WaveLineData( E_LINETYPE::RANDOM, E_ZOMBIE_TYPE::Zombie_Normal )
	);
	wavedata->m_ZombiDataVec.push_back(
		new WaveLineData(E_LINETYPE::RANDOM, E_ZOMBIE_TYPE::Zombie_Normal)
	);
	m_StageInfoData->AddWave(
		wavedata
	);


	//wavedata = new StageInfo_WaveElement( );
	//wavedata->m_ElapsedTime = 15.f;
	//wavedata->m_ZombiDataVec.push_back(
	//	new WaveLineData(E_LINETYPE::RANDOM, E_ZOMBIE_TYPE::Zombie_Normal)
	//);
	//m_StageInfoData->AddWave(
	//	wavedata
	//);

	//wavedata = new StageInfo_WaveElement( );
	//wavedata->m_ElapsedTime = 25.f;
	//wavedata->m_ZombiDataVec.push_back(
	//	new WaveLineData(E_LINETYPE::RANDOM, E_ZOMBIE_TYPE::Zombie_Normal)
	//);
	//m_StageInfoData->AddWave(
	//	wavedata
	//);



	StageWaveManager::GetI( )->SetStageInfoWaveData(m_StageInfoData
		, std::bind(&Stage01::CreateWaveZombi_CallFN, this, std::placeholders::_1, std::placeholders::_2) );
	//StageWaveManager::GetI( )->SetCreateWaveZombi_CallFN(
	//	std::bind(&Stage01::CreateWaveZombi_CallFN, this, std::placeholders::_1, std::placeholders::_2)
	//);
	StageWaveManager::GetI( )->Play( );
}

void Stage01::CreateWaveZombi_CallFN(StageInfo_WaveElement* p_element, int p_index)
{
	static Vec2 lineposarr[ ( int ) E_LINETYPE::LINE5 + 1 ] = {
		Vec2( 600, 100 ),
		Vec2( 600, 200 ),
		Vec2( 600, 300 ),
		Vec2( 600, 400 ),
		Vec2( 600, 500 )
	};


	//NormalZombie tempzom;


	ZombieFactoryManager* manager = ZombieFactoryManager::GetI( );
	Vec2 temppos;
	for ( auto* item : p_element->m_ZombiDataVec )
	{
		NormalZombie* zombi = manager->CreateZombie<NormalZombie>( );


		if ( item->E_LineType == E_LINETYPE::RANDOM )
		{
			temppos = lineposarr[ MyUtil::GetRandInt(0, ( int ) E_LINETYPE::LINE5)  ];
		}
		else
		{
			temppos = lineposarr[ rand( ) % ( int ) item->E_LineType ];
		}

		zombi->transform->SetDepth(( int ) E_ALLLayerType::Zombie);
		zombi->transform->SetWorldPosition(temppos);
	}
	

}
