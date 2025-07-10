#include "Transform.h"

void Transform::UpdateIfDirty( ) {
	if ( !isDirty ) return;

	if ( parent ) {
		cachedWorldPosition = parent->GetWorldPosition( );
		cachedWorldPosition.x += localposition.x;
		cachedWorldPosition.y += localposition.y;

		cachedWorldRotation = parent->GetWorldRotation( ) + rotation;

		auto parentScale = parent->GetWorldScale( );
		cachedWorldScale.x = parentScale.x * localScale.x;
		cachedWorldScale.y = parentScale.y * localScale.y;

		cachedWorldDepth = parent->GetWorldDepth( ) + zdepth;
	}
	else {
		cachedWorldPosition = localposition;
		//cachedWorldRotation = localRotation;
		//cachedWorldScale = localScale;
		cachedWorldDepth = zdepth;
	}

	// Build transformation matrix
	Gdiplus::Matrix matrix;
	matrix.Translate(cachedWorldPosition.x, cachedWorldPosition.y);
	matrix.Rotate(cachedWorldRotation);
	matrix.Scale(cachedWorldScale.x, cachedWorldScale.y);
	//cachedWorldMatrix = matrix;

	matrix.GetElements(m_tempcachedmatrix);
	cachedWorldMatrix.SetElements(
		m_tempcachedmatrix[ 0 ], m_tempcachedmatrix[ 1 ]
		, m_tempcachedmatrix[ 2 ], m_tempcachedmatrix[ 3 ]
		, m_tempcachedmatrix[ 4 ], m_tempcachedmatrix[ 5 ]
	);

	isDirty = false;
}

void Transform::SetWorldPosition(Vec2& p_wpos) 
{ 
	//if (parent) 
	//{
 //       Vec2 parentWorld = parent->GetWorldPosition();
 //       Vec2 newLocal = { p_wpos.x - parentWorld.x, p_wpos.y - parentWorld.y };

	//	setLocalPosition(newLocal);
 //   } 
	//else 
	//{
 //       setLocalPosition(p_wpos);
 //   }

	SetWorldPosition(p_wpos.x, p_wpos.y);
}

void Transform::SetWorldPosition(float p_x, float p_y) 
{
    if (parent) {
        Vec2 parentWorld = parent->GetWorldPosition();
        Vec2 newLocal = { p_x - parentWorld.x, p_y - parentWorld.y };

        setLocalPosition(newLocal);
    } else {
        setLocalPosition(p_x, p_y);
    }
}

void Transform::setLocalPosition(const Vec2& pos)
{
    localposition = pos;
    MarkDirty();
}

void Transform::setLocalPosition(float p_x, float p_y)
{
    localposition.x = p_x;
    localposition.y = p_y;
    MarkDirty();
}
