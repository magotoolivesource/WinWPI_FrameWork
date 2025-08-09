#pragma once
#include "Compoment/Component.h"
#include "Compoment/ImageComponent.h"
class BGImageCom : public Component
{
public:
	BGImageCom( );
	virtual ~BGImageCom( ) override;

protected:
	ImageComponent* m_BGComponent;



};

