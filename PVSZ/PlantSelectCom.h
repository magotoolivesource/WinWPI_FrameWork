#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
class PlantSelectCom : public Component
{
public:
	PlantSelectCom( );
	virtual ~PlantSelectCom( ) override;

protected:
	ImageComponent* m_BGComponent;



};

