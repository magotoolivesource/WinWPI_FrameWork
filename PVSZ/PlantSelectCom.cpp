#include "PlantSelectCom.h"
#include "Core/InputManager.h"
#include <cmath>
#include <algorithm>


#include "Sunflower_Com.h"
#include "Plant_PeaShooter.h"
#include "UI_TopSelectPanelCom.h"



PlantSelectCom::PlantSelectCom() : RectLineComponent(Gdiplus::RectF(0, 0, 100, 100))
, m_BGComponent(nullptr)
{
	rect.Width = m_OneBlockSize.x;
	rect.Height = m_OneBlockSize.y;

}


PlantSelectCom::~PlantSelectCom( )
{

}

void PlantSelectCom::Start()
{
	__super::Start();

}

void PlantSelectCom::Update(float dt)
{
	__super::Update(dt);


	// 시작 257, 82픽셀
	// 80, 98

	int offsetx = 257;
	int offsety = 82;

	POINT mpos = InputManager::GetMousePosition( );
	mpos.x -= offsetx;
	mpos.y -= offsety;
	//mpos.

	int gridindex_X = floorf( ( float ) mpos.x / m_OneBlockSize.x );
	int gridindex_Y = floorf( ( float ) mpos.y / m_OneBlockSize.y );

	if ( gridindex_X >= 0 && m_ISVisible <= 8
		&& gridindex_Y >= 0 && gridindex_Y <= 4
		)
	{
		m_ISVisible = true;
	}
	else
	{
		m_ISVisible = false;
	}

	m_GridIndex.x = std::clamp<int>(gridindex_X, 0, 8);
	m_GridIndex.y = std::clamp<int>(gridindex_Y, 0, 4);

	m_GridWPos.x = offsetx + (m_GridIndex.x * m_OneBlockSize.x);
	m_GridWPos.y = offsety + (m_GridIndex.y * m_OneBlockSize.y);

	transform->SetWorldPosition(m_GridWPos);


	if ( m_TopSelectPanelCom )
	{
		E_PlantType planttype = m_TopSelectPanelCom->GetPlantType( );
		if ( !(planttype == E_PlantType::None
			|| planttype == E_PlantType::MAX )
			)
		{
			if ( m_ISVisible && InputManager::ISMouseDown(E_MOUSEETYPE::LEFT) )
			{
				if ( planttype == E_PlantType::SUNFLOWER )
				{
					Sunflower_Com* sunflowercom = Sunflower_Com::CreateSunflower_Com( );
					sunflowercom->transform->SetWorldPosition(m_GridWPos);
				}
				else if( planttype == E_PlantType::PEASHOOTER )
				{
					Plant_PeaShooter* sunflowercom = Plant_PeaShooter::Create_PeaShooterObject( );
					sunflowercom->transform->SetWorldPosition(m_GridWPos);
				}
				else if ( planttype == E_PlantType::SNOWPEASHOOTER )
				{
					
				}
				else if ( planttype == E_PlantType::POTATO )
				{
					
				}

			}

		}

	}
	

}

void PlantSelectCom::Render(HDC hdc)
{
	if ( m_TopSelectPanelCom )
	{
		E_PlantType planttype = m_TopSelectPanelCom->GetPlantType( );
		if ( !( planttype == E_PlantType::None
			|| planttype == E_PlantType::MAX )
			)
		{
			if ( m_ISVisible )
			{
				__super::Render(hdc);
			}
			
		}
	}
	


}
