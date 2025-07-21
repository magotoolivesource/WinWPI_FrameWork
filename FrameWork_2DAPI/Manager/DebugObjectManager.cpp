#include "DebugObjectManager.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>
#include <type_traits>
#include <string>

//#include "../Core/SingletonT.h"
//#include "../Compoment/Camera.h"

#include "../Core/GameObject.h"
#include "../Core/DebugObject.h"
#include "../Core/DebugLineComponent.h"
#include "../Core/DebugRectLineComponent.h"



void DebugObjectManager::AddDebugRect_ResizeCount(int p_addsize) 
{ 
	int size = p_addsize; // m_RectLineCount;
	//DebugRectLineComponent* rectline = nullptr;
	//Gdiplus::Image* img = new Gdiplus::Image(path.c_str());
	for (size_t i = 0; i < size; i++) 
	{ 
		auto rectline = std::make_unique<DebugRectLineComponent>();
        rectline.get()->SetActive(false);
        m_DebugRectLineVec.emplace_back(std::move(rectline));
        //m_DebugRectLineVec.push_back(rectline);

		// 
		// 추가 적용
		//m_DebugObjects.push_back( rectline.get() );
	}
	
	m_CacheRectSize = m_DebugRectLineVec.size();
}

void DebugObjectManager::AddDebugLine_ResizeCount(int p_addsize) 
{
    int size = p_addsize;//    m_LineCount;
    for (size_t i = 0; i < size; i++) {
        auto line = std::make_unique<DebugLineComponent>();
        line.get()->SetActive(false);
        m_DebugLineVec.emplace_back(std::move(line));

		// 추가 적용
        //m_DebugObjects.push_back( rectline.get() );
    }

	m_CacheLineSize = m_DebugLineVec.size();
}

DebugObjectManager::~DebugObjectManager() 
{ 

	for (size_t i = 0; i < m_DebugObjects.size(); i++) 
	{
        m_DebugObjects[i] = nullptr;
    }
	m_DebugObjects.clear();


	for (size_t i = 0; i < m_DebugRectLineVec.size(); i++) 
	{ 
		m_DebugRectLineVec[i].release();
	}
	m_DebugRectLineVec.clear();
}

DebugLineComponent* DebugObjectManager::DrawLine(Vec2& p_st, Vec2& p_end, float p_thickness, Gdiplus::Color color) { 

	if (m_LineCount >= m_DebugLineVec.size())
	{
        AddDebugLine_ResizeCount(m_CacheLineSize); 
	}
	
	DebugLineComponent* outline = m_DebugLineVec[m_LineCount++].get();
    if (outline == nullptr) 
		return nullptr;

	outline->SetLine(p_st, p_end);
	outline->SetColor(color);
	outline->SetThickness(p_thickness);
    outline->SetActive(true);

	m_DebugObjects.push_back(outline);

	return outline;
}

DebugRectLineComponent* DebugObjectManager::DrawRectLine(Vec2& p_min, Vec2& p_max
	, float p_thickness, Gdiplus::Color color) 
{
    if (m_RectLineCount >= m_DebugRectLineVec.size())
	{
        AddDebugRect_ResizeCount(m_CacheRectSize); 
	}

	DebugRectLineComponent* outrect = m_DebugRectLineVec[m_RectLineCount++].get();
	if (outrect == nullptr) 
	{
        return nullptr;
    }

    outrect->SetRect(p_min, p_max);
    outrect->SetColor(color);
    outrect->SetThickness(p_thickness);
	outrect->SetActive(true);


	m_DebugObjects.push_back(outrect);

	//m_DebugRectLineVec[m_RectLineCount++]->SetActive(true);

	return outrect;
}

DebugRectLineComponent* DebugObjectManager::DrawRectLine( float p_x, float p_y
	, float p_width, float p_height
	, float p_thickness
	, Gdiplus::Color color) 
{

	if (m_RectLineCount >= m_DebugRectLineVec.size()) {
        AddDebugRect_ResizeCount(m_CacheRectSize);
    }

    DebugRectLineComponent* outrect = m_DebugRectLineVec[m_RectLineCount++].get();
    if (outrect == nullptr) {
        return nullptr;
    }

    outrect->SetRect(p_x, p_y, p_width, p_height);
    outrect->SetColor(color);
    outrect->SetThickness(p_thickness);
    outrect->SetActive(true);

    m_DebugObjects.push_back(outrect);

    return outrect;
}

void DebugObjectManager::ResetAllDebugObjects() 
{
    //for (auto i = 0; i < length; i++) { }
	for (auto& debugObject : m_DebugObjects) {
		if (debugObject) {
			debugObject->SetActive(false);
		}
    }
    m_DebugObjects.clear();

	m_RectLineCount = 0;
    m_LineCount = 0;
}

void DebugObjectManager::AllDebugUpdate(float dt) 
{ 
	for (auto& debugObject : m_DebugObjects) {
        if (debugObject ) {
            debugObject->Update(dt);
        }
    }
}

void DebugObjectManager::AllDebugRender(HDC p_hdc) 
{ 
	if (m_ISAllDebug == false) 
		return;

	for (auto& debugObject : m_DebugObjects) 
	{
        if (debugObject && debugObject->IsActive() ) 
		{
            debugObject->Render(p_hdc);
        }
    }

}

void DebugObjectManager::Initialize() 
{

}
