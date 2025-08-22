#pragma once
#include "Component.h"
#include "ImageComponent.h"
#include <vector>
#include <string>
#include "../Core/DefineHeader.h"

using namespace std;

class ImageComponent;

class DrawImageRectInfoData
{
	//Gdiplus::Image* m_Image;
public:
	DrawImageRectInfoData( ) {}
	virtual ~DrawImageRectInfoData( ) {};
public:
	float m_DurationSec = 0.f;
	std::wstring m_ImagePath = L"";
	int drawWidth = 0, drawHeight = 0;
	RECT drawRect = { 0, 0, 128, 128 };
	float OffsetPosX = 0, OffsetPosY = 0;
};

class ImgCom_DrawImageRectInfoData : public DrawImageRectInfoData
{
public:
	ImgCom_DrawImageRectInfoData(){}
	ImgCom_DrawImageRectInfoData(DrawImageRectInfoData& p_data)
	{
		SetDrawInfoData(p_data);
	}
	virtual ~ImgCom_DrawImageRectInfoData( )
	{

	}
	void SetDrawInfoData(DrawImageRectInfoData& p_data)
	{
		m_DurationSec = p_data.m_DurationSec;
		m_ImagePath = p_data.m_ImagePath;
		drawWidth = p_data.drawWidth;
		drawHeight = p_data.drawHeight;
		drawRect = p_data.drawRect;
		OffsetPosX = p_data.OffsetPosX;
		OffsetPosY = p_data.OffsetPosY;
	}
public:
	Gdiplus::Image* m_Image;

	void AdjustImageLoad( );
};

// 참조로 사용할지 상속으로 사용할지 생각하기 우선 참조적용
class ImageAni_Component : public Component
{
public:
	ImageAni_Component();
	virtual ~ImageAni_Component( ) override;

public:
	void InitSettings( );
	virtual void Initialize_AddCompoment( ) override;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	//virtual void Render(HDC hdc) override;

public:
	void AddDrawImageInfo(DrawImageRectInfoData* p_data);
	void AddDrawImageInfo(float p_durationsec, wstring p_imgpath, int p_draww, int p_drawh
		, RECT p_rect
		, float p_offsetx, float p_offsety);
	void AddDrawImageInfo(float p_durationsec, wstring p_imgpath, int p_draww, int p_drawh
		, float p_left, float p_top, float p_right, float p_bottom
		, float p_offsetx, float p_offsety);

	void SwapDrawInfoAt(int p_srcindex, int p_destindex);
	void RemoveDrawInfoAt(int p_at);

	void AdjustImageInfo( );
	void SetNAdjustImageInfoTotalFrame(float p_totalanisec);

protected:
	vector<unique_ptr<DrawImageRectInfoData>> m_AllDrawImaTableVec;
	//int m_LoopTime = -1; 
	GETSETER(int, LoopTime, -1); // -1이면 무한루프, 0이면 한번만 실행, 양수면 해당 횟수만큼 반복
	GETSETER(float, Weight_AnimationVal, 1.f); // 가중치 값, 0이면 안보임, 1이면 기본값, 2면 두배로 보임 등등

protected:
	ImgCom_DrawImageRectInfoData* m_CurrentIamgeInfoData;
	float m_RemineSec = 0.f; // 남은 시간
	int m_CurrentIndex = 0; // 현재 인덱스
	int m_RemineLoopCount = 1;
	int m_PrevIndex = -1;

	vector<unique_ptr<ImgCom_DrawImageRectInfoData>> m_AllImageAniComVec; // 이미지 컴포넌트 벡터

public:
	void ResetAllDatasNClear( );
	void SetCurrentIndex(int p_index);
	void SetRemineSec(float p_reminesec);
	GETSETER(bool, ISPlayAni, false);

	void Play( );
	void Pause( );

protected:
	void SetNextDatas( float p_reminesec );

protected:
	ImageComponent* m_LinkImageCom;
	void SetUpdateImageComponent( );
};

