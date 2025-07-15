#include "Transform.h"
#include "Camera.h"

void Transform::UpdateIfDirty( ) {
	if ( !isDirty ) return;

	
	if ( parent ) {

		// 회전
        auto parentRotation = parent->GetWorldRotation();
        cachedWorldRotation = parentRotation + local_rotation;

		// 스케일
        auto parentScale = parent->GetWorldScale();
        cachedWorldScale.x = parentScale.x * localScale.x;
        cachedWorldScale.y = parentScale.y * localScale.y;

		//cachedPivotPos = PivotPos;

		// 상위 위치를 기준으로한 현재 중심축 위치 변환용
		m_TempCalcMatrix.Reset();
        m_TempCalcMatrix.Translate(localposition.x, localposition.y, Gdiplus::MatrixOrderAppend);
        m_TempCalcMatrix.Rotate(parentRotation, Gdiplus::MatrixOrderAppend);
        m_TempCalcMatrix.Scale(parentScale.x, parentScale.y, Gdiplus::MatrixOrderAppend);

        m_TempCalcMatrix.GetElements(m_tempcachedmatrix);

		cachedPivotPos.x = PivotPos.x * parentScale.x;
        cachedPivotPos.y = PivotPos.y * parentScale.y;

		cachedWorldPosition = parent->GetWorldPosition( );
        cachedWorldPosition.x += m_tempcachedmatrix[(int)MatrixElements::dx]; // localposition.x;
        cachedWorldPosition.y += m_tempcachedmatrix[(int)MatrixElements::dy]; //localposition.y;


		cachedWorldDepth = parent->GetWorldDepth( ) + zdepth;
	}
	else {
		cachedWorldPosition = localposition;
		cachedWorldRotation = local_rotation;
		cachedWorldScale = localScale;
		cachedWorldDepth = zdepth;

		cachedPivotPos = PivotPos;
	}

	//// Build transformation matrix
	//Gdiplus::Matrix matrix;
	//matrix.Translate(cachedWorldPosition.x, cachedWorldPosition.y);
	//matrix.Rotate(cachedWorldRotation);
	//matrix.Scale(cachedWorldScale.x, cachedWorldScale.y);
	////cachedWorldMatrix = matrix;

	////m_tempcachedmatrix.Translate

	//matrix.GetElements(m_tempcachedmatrix);
	//cachedWorldMatrix.SetElements(
	//	m_tempcachedmatrix[ 0 ], m_tempcachedmatrix[ 1 ]
	//	, m_tempcachedmatrix[ 2 ], m_tempcachedmatrix[ 3 ]
	//	, m_tempcachedmatrix[ 4 ], m_tempcachedmatrix[ 5 ]
	//);

	cachedWorldMatrix.Reset();
	if ( true )
	{
        // scale -> -pivotpos -> rotate -> wpos 순서로 연산하면 스케일 적용전 중심위치가 월드상으로 바뀌게되서 중심 위치가 바뀜
		// -pivotpos -> scale -> rotate -> wpos 순서로 적용하고 있음
		// -pivotpos -> scale -> rotate -> povotpos -> wpos 순서로 적용하면 중심위치 원위치로 바꿈 저렇게 하면 중심위치 원상태로 복구안되고 이상하게 처리될수 있음
		// 
		
        // Gdiplus::MatrixOrderAppend 코드에서 이동 처리용
        cachedWorldMatrix.Translate(cachedPivotPos.x, cachedPivotPos.y, Gdiplus::MatrixOrderAppend);
        cachedWorldMatrix.Scale(cachedWorldScale.x, cachedWorldScale.y, Gdiplus::MatrixOrderAppend);
        cachedWorldMatrix.Rotate(cachedWorldRotation, Gdiplus::MatrixOrderAppend);
        //cachedWorldMatrix.Translate(PivotPos.x, PivotPos.y, Gdiplus::MatrixOrderAppend);
        cachedWorldMatrix.Translate(cachedWorldPosition.x, cachedWorldPosition.y, Gdiplus::MatrixOrderAppend);    

	}
	else
	{
        // MatrixOrderPrepend 역순방식 연산
        cachedWorldMatrix.Translate(cachedWorldPosition.x, cachedWorldPosition.y);
        cachedWorldMatrix.Translate(PivotPos.x, PivotPos.y);
        cachedWorldMatrix.Rotate(cachedWorldRotation);
        cachedWorldMatrix.Scale(cachedWorldScale.x, cachedWorldScale.y);
        cachedWorldMatrix.Translate(-PivotPos.x, -PivotPos.y);
	}

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
