#include "PlantSelectCom.h"
#include "Core/InputManager.h"
#include <cmath>
#include <algorithm>

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

	m_GridIndex.x = floorf( ( float ) mpos.x / m_OneBlockSize.x );
	m_GridIndex.y = floorf( ( float ) mpos.y / m_OneBlockSize.y );


	m_GridIndex.x = std::clamp<int>(m_GridIndex.x, 0, 8);
	m_GridIndex.y = std::clamp<int>(m_GridIndex.y, 0, 4);

	m_GridWPos.x = offsetx + (m_GridIndex.x * m_OneBlockSize.x);
	m_GridWPos.y = offsety + (m_GridIndex.y * m_OneBlockSize.y);

	transform->SetWorldPosition(m_GridWPos);
}

void PlantSelectCom::Render(HDC hdc)
{
	__super::Render(hdc);


}
