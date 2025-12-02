#include "ImageAni_Component.h"
#include <cmath>
#include <algorithm>
#include <format>
#include <string>
#include <fcntl.h>


#include "../Core/GameObject.h"
#include "../Core/UtilLoger.h"
#include "../Compoment/ImageComponent.h"

#include "../Manager/ImageManager.h"

using namespace std;

ImageAni_Component::ImageAni_Component( )
{
}

ImageAni_Component::~ImageAni_Component( )
{

}

void ImageAni_Component::Start()
{
	__super::Start();

	m_LinkImageCom = this->owner->GetComponent<ImageComponent>( );

	SetCurrentIndex(m_CurrentIndex);
}

void ImageAni_Component::Update(float dt)
{
	__super::Update(dt);


	if ( !m_ISPlayAni )
		return;


	m_RemineSec -= (dt * m_Weight_AnimationVal);

	if ( m_RemineSec <= 0 )
	{
		SetNextDatas(m_RemineSec);
	}

	if ( !m_CurrentIamgeInfoData )
	{
		return;
	}

	//SetUpdateImageComponent( );
}

//void ImageAni_Component::Render(HDC hdc)
//{
//	__super::Render(hdc);
//
//
//}

void ImageAni_Component::AddDrawImageInfo(DrawImageRectInfoData* p_data)
{
}

void ImageAni_Component::AddDrawImageInfo(float p_delaysec
	, wstring p_imgpath
	, int p_draww, int p_drawh
	, RECT p_rect
	, float p_offsetx, float p_offsety)
{
	// 추가하기
	std::unique_ptr<DrawImageRectInfoData> infodata = std::make_unique<DrawImageRectInfoData>( );
	infodata->m_ImagePath = p_imgpath;
	infodata->drawWidth = p_draww;
	infodata->drawHeight = p_drawh;
	infodata->drawRect = p_rect;
	infodata->OffsetPosX = p_offsetx;
	infodata->OffsetPosY = p_offsety;

	m_AllDrawImaTableVec.emplace_back(std::move(infodata));

}

void ImageAni_Component::AddDrawImageInfo(float p_delaysec
	, wstring p_imgpath
	, int p_draww, int p_drawh
	, float p_left, float p_top, float p_right, float p_bottom
	, float p_offsetx, float p_offsety)
{
	std::unique_ptr<DrawImageRectInfoData> infodata = std::make_unique<DrawImageRectInfoData>( );
	infodata->m_ImagePath = p_imgpath;
	infodata->drawWidth = p_draww;
	infodata->drawHeight = p_drawh;
	infodata->drawRect = { (int)p_left, ( int ) p_top, ( int ) p_right, ( int ) p_bottom };
	infodata->OffsetPosX = p_offsetx;
	infodata->OffsetPosY = p_offsety;

	m_AllDrawImaTableVec.emplace_back(std::move(infodata));
}

void ImageAni_Component::AddDrawImageInfo(wstring p_aniname, float p_durationsec, wstring p_imgpath, int p_draww, int p_drawh, float p_left, float p_top, float p_right, float p_bottom, float p_offsetx, float p_offsety)
{

}

void ImageAni_Component::SwapDrawInfoAt(int p_srcindex, int p_destindex)
{
	if ( !(p_srcindex >= 0 && p_srcindex < m_AllDrawImaTableVec.size( ) &&
		p_destindex >= 0 && p_destindex < m_AllDrawImaTableVec.size( ) )
		)
	{
		return;
	}

	std::swap(m_AllDrawImaTableVec[ p_srcindex ], m_AllDrawImaTableVec[ p_destindex ]);
	AdjustImageInfo( );
}

void ImageAni_Component::RemoveDrawInfoAt(int p_at)
{
	if ( p_at < 0 && p_at >= m_AllDrawImaTableVec.size( ) )
	{
		return;
	}

	m_AllDrawImaTableVec.erase(m_AllDrawImaTableVec.begin( ) + p_at);
	AdjustImageInfo( );
}

void ImageAni_Component::AdjustImageInfo( )
{
	int size = m_AllDrawImaTableVec.size( );
	m_AllImageAniComVec.resize( size );

	ImgCom_DrawImageRectInfoData* tempdata = nullptr;
	for ( size_t i = 0; i < size; i++ )
	{
		tempdata = m_AllImageAniComVec[ i ].get();
		if ( !tempdata )
		{
			auto tempinfodata = std::make_unique<ImgCom_DrawImageRectInfoData>();
			m_AllImageAniComVec[ i ] = move(tempinfodata);
			tempdata = m_AllImageAniComVec[ i ].get();
		}
		
		tempdata->SetDrawInfoData( *m_AllDrawImaTableVec[ i ] );
		tempdata->AdjustImageLoad( );
	}

}

void ImageAni_Component::SetNAdjustImageInfoTotalFrame(float p_totalanisec)
{
	if ( p_totalanisec < 0.f )
	{
		_ASSERT(false && "p_totalanisec 값이 0이하입니다.");
		return;
	}

	int size = m_AllDrawImaTableVec.size( );
	if ( size <= 0 )
	{
		_ASSERT(false && "m_AllDrawImaTableVec 값이 없습니다.");
		return;
	}
	
	float divsec = p_totalanisec / ( float ) size;
	for ( auto& item : m_AllDrawImaTableVec )
	{
		item.get( )->m_DurationSec = divsec;
	}

	AdjustImageInfo( );
}

void ImageAni_Component::SetChangeAnimation(wstring p_aniname, int p_looptime, int p_stindex)
{
}

void ImageAni_Component::ResetAllDatasNClear( )
{
	m_AllDrawImaTableVec.clear( );
	m_AllImageAniComVec.clear( );

	m_CurrentIamgeInfoData = nullptr;
	m_RemineSec = 0.f;
	m_CurrentIndex = 0;
	m_RemineLoopCount = 1;
	m_PrevIndex = -1;

	m_LoopTime = -1;
	m_Weight_AnimationVal = 1.f;

	//SetUpdateImageComponent( );
}

void ImageAni_Component::SetCurrentIndex(int p_index)
{
	if ( p_index < 0 && p_index >= m_AllImageAniComVec.size( ) )
		return;

	m_CurrentIndex = p_index;
	m_CurrentIamgeInfoData = m_AllImageAniComVec[ m_CurrentIndex ].get();
	m_RemineSec = m_CurrentIamgeInfoData->m_DurationSec;

	SetUpdateImageComponent( );
}

void ImageAni_Component::SetRemineSec(float p_reminesec)
{
	m_RemineSec = p_reminesec;
}

void ImageAni_Component::SetNextDatas(float p_reminesec)
{
	m_CurrentIndex += 1;

	if ( m_CurrentIndex >= m_AllImageAniComVec.size( ) )
	{
		--m_RemineLoopCount;

		if ( m_LoopTime <= -1 )
		{
			m_RemineLoopCount = 1;
		}

		if ( m_RemineLoopCount > 0 )
		{
			m_CurrentIndex = 0;
		}
		else
		{
			--m_CurrentIndex;
		}
	}

	m_CurrentIamgeInfoData = m_AllImageAniComVec[ m_CurrentIndex ].get();
	m_RemineSec = m_CurrentIamgeInfoData->m_DurationSec + p_reminesec;

	SetUpdateImageComponent( );
}

void ImageAni_Component::SetUpdateImageComponent( )
{
	if ( !m_CurrentIamgeInfoData )
	{
		return;
	}

	if ( !m_LinkImageCom )
		return;

	m_LinkImageCom->SetImage(m_CurrentIamgeInfoData->m_Image);

	m_LinkImageCom->SetDrawRect(m_CurrentIamgeInfoData->drawWidth, m_CurrentIamgeInfoData->drawHeight
		, m_CurrentIamgeInfoData->drawRect.left
		, m_CurrentIamgeInfoData->drawRect.top
		, m_CurrentIamgeInfoData->drawRect.right
		, m_CurrentIamgeInfoData->drawRect.bottom
		, m_CurrentIamgeInfoData->OffsetPosX
		, m_CurrentIamgeInfoData->OffsetPosY
	);
}


void ImageAni_Component::InitSettings( )
{
	
}

void ImageAni_Component::Initialize_AddCompoment( )
{
	__super::Initialize_AddCompoment( );

	InitSettings( );
}

void ImgCom_DrawImageRectInfoData::AdjustImageLoad( )
{
	m_Image = ImageManager::GetI( )->Load(m_ImagePath);

}
