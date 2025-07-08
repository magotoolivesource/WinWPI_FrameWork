#pragma once
#include "Component.h"
#include "../Core/Vector.h"

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
    Vec2 position;
	Vec2 world_position;
	float zdepth = 0; // Z-order for rendering
    float width = 50.0f;
    float height = 50.0f;
    float rotation = 0.0f; // in degrees


private:
	Transform* parent = nullptr;
	std::vector<Transform*> children;

public:
	void SetParent(Transform* newParent) {
		if ( parent ) {
			auto& siblings = parent->children;
			siblings.erase(std::remove(siblings.begin( ), siblings.end( ), this), siblings.end( ));
		}

		parent = newParent;
		if ( newParent ) {
			newParent->children.push_back(this);
		}
	}

	Vec2 GetWorldPosition( ) const {
		if ( !parent ) return position;
		auto p = parent->GetWorldPosition( );
		return { p.x + position.x, p.y + position.y };
	}
	float GetWorldDepth( ) const {
		if ( !parent ) return zdepth;
		return parent->GetWorldDepth( ) + zdepth;
	}
	void UpdateAllPositions( )
	{
		// 현재 위치 값 바뀌면 내 위치에서부터 하단값들 무조건 바꾸도록 처리하기
	}

public:
    // Getters
    Vec2 getPosition() const { return position; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getRotation() const { return rotation; }
    // Setters
    void setPosition(const Vec2& pos) { position = pos; }
    void setPosition(int p_x, int p_y) 
    { 
        position.x = p_x;
        position.y = p_y;
    }
    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
    void setRotation(float rot) { rotation = rot; }
    // Override the clone method
	//virtual Transform* clone() const override { return new Transform(*this); }
};

