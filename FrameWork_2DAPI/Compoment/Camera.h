#pragma once
#include "../Core/Rect.h"
#include "Component.h"
//#include "Transform.h"
#include <windows.h>
#include <gdiplus.h>

class Camera : public Component 
{

protected:
    //Transform* transform;
    float zoom = 1.0f;


public:
    void SetLocalPosition(const Vec2& pos);
    void SetLocalPosition(int p_x, int p_y);
    void SetZoom(float z) { zoom = z; }

	Vec2 ApplyToPoint(const Vec2& worldPos) const;

    void ApplyToMatrix(Gdiplus::Matrix& matrix) const;

public:
	Camera() = default;
	virtual ~Camera();


	virtual void Start() override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;
	virtual void Initialize_AddCompoment() override;
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Destroy() override;

	//// Getters
	//Transform* GetTransform() const { return transform; }
 //   Rect GetViewport() const { return viewport; }

	//void SetViewport(const Rect& rect) { viewport = rect; }
	//void SetViewport(int x, int y, int width, int height) {
	//	viewport = Rect{ x, y, width, height }; }

public:

};
