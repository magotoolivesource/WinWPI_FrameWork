#pragma once
#include "Component.h"
#include "../Core/Vector.h"

#include <cmath>
#include <algorithm>
#include <windows.h>
#include <gdiplus.h>



class Transform : public Component
{
    // new Transform 막기위한것
protected:
    Transform() = default;
    friend class GameObject;

    ////float x = 0, y = 0;
    ////float rotation = 0;
    ////float scaleX = 1, scaleY = 1;

    //float width = 50.0f;
    //float height = 50.0f;
    //float rotation = 0.0f; // in degrees

public:
    Vec2 localposition;
	//Vec2 world_position;
	float zdepth = 0; // Z-order for rendering
    float width = 50.0f;
    float height = 50.0f;
    float rotation = 0.0f; // in degrees


	Vec2 localScale = { 1, 1 };

private:
	Transform* parent = nullptr;
	std::vector<Transform*> children;


	bool isDirty = true;
	Vec2 cachedWorldPosition = { 0, 0 };
	float cachedWorldRotation = 0.0f;
	Vec2 cachedWorldScale = { 1, 1 };
	float cachedWorldDepth = 0.0f;
	
	Gdiplus::Matrix cachedWorldMatrix;

	float m_tempcachedmatrix[ 6 ] = { 0, 0, 0, 0, 0, 0 };
protected:
	void MarkDirty( ) {
		isDirty = true;
		for ( auto child : children ) {
			child->MarkDirty( );
		}
	}

	void UpdateIfDirty( );

public:
	void SetDepth(float d) {
		zdepth = d;
		MarkDirty( );
	}

	float GetDepth( ) const {
		//UpdateIfDirty( );
		return zdepth;
	}

	void SetParent(Transform* newParent) {
		if ( parent ) {
			auto& siblings = parent->children;
			siblings.erase(std::remove(siblings.begin( ), siblings.end( ), this), siblings.end( ));
		}

		parent = newParent;
		if ( newParent ) {
			newParent->children.push_back(this);
		}

		MarkDirty( );
	}


	float GetWorldDepth( ) {
		UpdateIfDirty( );
		return cachedWorldDepth;
	}

	Gdiplus::Matrix& GetWorldMatrix( ) {
		UpdateIfDirty( );
		return cachedWorldMatrix;
	}


	void SetWorldPosition(Vec2& p_wpos);
    void SetWorldPosition(float p_x, float p_y);


	Vec2& GetWorldPosition( ) {
		//if ( !parent ) return localposition;
		//auto p = parent->GetWorldPosition( );
		//return { p.x + localposition.x, p.y + localposition.y };

		//if ( isDirty ) {
		//	if ( parent ) {
		//		auto p = parent->GetWorldPosition( );
		//		cachedWorldPosition = { p.x + localposition.x, p.y + localposition.y };
		//	}
		//	else {
		//		cachedWorldPosition = localposition;
		//	}
		//	isDirty = false;
		//}

		UpdateIfDirty( );
		return cachedWorldPosition;
	}


	//float GetWorldDepth( ) {
	//	//if ( !parent ) return zdepth;
	//	//return parent->GetWorldDepth( ) + zdepth;

	//	if ( isDirty ) {
	//		if ( parent ) {
	//			cachedWorldDepth = parent->GetWorldDepth( ) + zdepth;
	//		}
	//		else {
	//			cachedWorldDepth = zdepth;
	//		}
	//	}
	//	return cachedWorldDepth;
	//}

	void UpdateAllPositions( )
	{
		// 현재 위치 값 바뀌면 내 위치에서부터 하단값들 무조건 바꾸도록 처리하기
	}

public:
    // Getters
    Vec2 getLocalPosition() const { return localposition; }
    // Setters
    void setLocalPosition(const Vec2& pos);
    void setLocalPosition(float p_x, float p_y);

	float getWidth( ) const { return width; }
	float getHeight( ) const { return height; }
    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }


	//void SetDepth(float d) {
	//	zdepth = d;
	//	MarkDirty( );
	//}

	float getRotation( ) const { return rotation; }
 //   void setRotation(float rot) 
	//{ 
	//	rotation = rot; 
	//	MarkDirty( );
	//}
    // Override the clone method
	//virtual Transform* clone() const override { return new Transform(*this); }
	void SetLocalRotation(float angleDegrees) {
		rotation = angleDegrees;
		MarkDirty( );
	}
    float GetWorldRotation() {
        UpdateIfDirty();
        return cachedWorldRotation;
    }
    void SetWorldRotation(float worldAngle) {
        if (parent) {
            SetLocalRotation(worldAngle - parent->GetWorldRotation());
        } else {
            SetLocalRotation(worldAngle);
        }
    }


	void SetLocalScale(Vec2 scale) {
		localScale = scale;
		MarkDirty( );
	}
    void SetLocalScale(float p_scalex, float p_scaley) {
        localScale.x = p_scalex;
        localScale.y = p_scaley;
        MarkDirty();
    }
    Vec2& GetWorldScale() {
        UpdateIfDirty();
        return cachedWorldScale;
    }
    void SetWorldScale(Vec2& worldScale) {
        if (parent) {
            Vec2 parentScale = parent->GetWorldScale();
            Vec2 newLocal = { worldScale.x / parentScale.x, worldScale.y / parentScale.y };
            SetLocalScale(newLocal);
        } else {
            SetLocalScale(worldScale);
        }
    }
};

