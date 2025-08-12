#pragma once
#include <vector>
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
#include "Compoment/RectLineComponent.h"
#include "Core/Vector.h"
#include "PVSZ_AllContentDatas.h"

#include <Core/DefineHeader.h>


class UI_SelectBTNCom;
class Button;

class UI_TopSelectPanelCom : public Component
{
public:
	UI_TopSelectPanelCom();
	virtual ~UI_TopSelectPanelCom( ) override;

protected:
	ImageComponent* m_BGTopUIImage;
	ImageComponent* m_BGSelectCom;

	ImageComponent* m_BGIconCom;


	std::vector< UI_SelectBTNCom*> m_SelectBTNComVec;

public:
	void InitSettings( );

public:
	virtual void Start( ) override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;

protected:
	void Click_CallBackFN(Button* p_clickbtn, int p_selectid);

protected:
	Component* m_SelectComponent;

	Vec2 m_OneBlockSize = { 80, 98 };

	Vec2 m_GridIndex;
	Vec2 m_GridWPos;


protected:
	UI_SelectBTNCom* m_CurrentSelectBTN = nullptr;
	//E_PlantType m_PlantType = E_PlantType::MAX;
	GETSETER(E_PlantType, PlantType, E_PlantType::MAX);

public:
	UI_SelectBTNCom* GetCurrentSelectBTNCom( );


};

