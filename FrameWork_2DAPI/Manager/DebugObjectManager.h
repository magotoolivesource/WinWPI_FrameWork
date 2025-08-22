#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>
#include <type_traits>
#include <string>

#include "../Core/SingletonT.h"
#include "../Core/GameObject.h"
#include "../Core/Vector.h"
//#include "../Core/DebugObject.h"
//#include "../Core/DebugObject.h"

#include "../Core/DebugLineComponent.h"
#include "../Core/DebugRectLineComponent.h"
#include "../Core/DebugText.h"

class DebugObject;
class DebugLineComponent;
class DebugRectLineComponent;
class DebugText;

class DebugObjectManager : public SingletonT<DebugObjectManager> {


protected:
    bool m_ISAllDebug = true;

public:
	bool IsAllDebug() const { return m_ISAllDebug; }
    void SetAllDebug(bool v) { m_ISAllDebug = v; }

protected:
	//std::unordered_map<std::type_index, std::unique_ptr<GameObject>> m_AllCameraMap;
    std::vector<DebugObject*> m_DebugObjects;

	int m_RectLineCount = 0;
    int m_CacheRectSize = 4;
	std::vector<std::unique_ptr<DebugRectLineComponent>> m_DebugRectLineVec;

	int m_LineCount = 0;
    int m_CacheLineSize = 4;
	std::vector<std::unique_ptr<DebugLineComponent>> m_DebugLineVec;
	

	int m_DebugTextCount = 0;
    int m_CacheDebugTextSize = 4;
    std::vector<std::unique_ptr<DebugText>> m_DebugTextVec;

protected:
    void AddDebugRect_ResizeCount(int p_addsize);
    void AddDebugLine_ResizeCount(int p_addsize);
    void AddDebugText_ResizeCount(int p_addsize);

public:
    DebugObjectManager() = default;
    virtual ~DebugObjectManager();

	
	DebugLineComponent* DrawLine(Vec2& p_st, Vec2& p_end, float p_thickness = 1.f, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0));
    DebugRectLineComponent* DrawRectLine( Vec2& p_min, Vec2& p_max, float p_thickness = 1.f, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0));
    DebugRectLineComponent* DrawRectLine( float p_x, float p_y
		, float p_width, float p_height
		, float p_thickness = 1.f
		, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0)
	);

	DebugText* DrawDebugText(std::wstring p_str, Vec2& p_pos, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0) );
	DebugText* DrawDebugText(std::string p_str, Vec2& p_pos, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0));
public:
    void ResetAllDebugObjects();
    void AllDebugUpdate(float dt);
    void AllDebugRender(HDC p_hdc);

	virtual void Initialize() override;
};