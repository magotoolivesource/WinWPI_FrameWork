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
	// Rect viewport; // Viewport in world coordinates
	// Gdiplus::Matrix cameraMatrix; // Camera transformation matrix
	// Cached values for performance
	Vec2 cachedWorldPosition = { 0, 0 };
	float cachedWorldRotation = 0.0f;
	Vec2 cachedWorldScale = { 1, 1 };
    float cachedWorldDepth = 0.0f;


	bool isDirty = true;
    Gdiplus::Matrix cachedWorldMatrix;
    //Gdiplus::Matrix* m_TransformMatrix;
    //Gdiplus::Matrix m_TempCalcMatrix;

public:
    void MarkDirty() {
        isDirty = true;
    }

    void UpdateIfDirty();

	Gdiplus::Matrix& GetWorldMatrix();
    float GetWorldRotation() const;
    Vec2& GetWorldPos() const;
    float GetZoom() { return zoom; }	


	// 카메라 가운데를 기준으로 회전 하도록 처리하기 
    void SetWorldRotation(float p_rot);

public:
    void SetWorldPosition( Vec2& pos);
    void SetWorldPosition(int p_x, int p_y);
    void SetLocalPosition(const Vec2& pos);
    void SetLocalPosition(int p_x, int p_y);
    void SetZoom(float z) { zoom = z; }

	Vec2 ApplyToPoint(const Vec2& worldPos) const;
    void ApplyToMatrix(Gdiplus::Matrix& matrix) const;

	Gdiplus::Matrix& GetMultiplyMatrix( Gdiplus::Matrix& matrix);

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
