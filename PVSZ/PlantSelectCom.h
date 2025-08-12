#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"



class UI_TopSelectPanelCom;

class PlantSelectCom : public RectLineComponent
{
public:
	PlantSelectCom();
	virtual ~PlantSelectCom( ) override;

protected:
	ImageComponent* m_BGComponent;

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;


protected:
	Vec2 m_OneBlockSize = { 80, 98 };

	Vec2 m_GridIndex;
	Vec2 m_GridWPos;
	bool m_ISVisible = false;
public:
	UI_TopSelectPanelCom* m_TopSelectPanelCom = nullptr;

};

