#include "MyString.h"

using namespace Gdiplus;



//
////-----------------------------------------------------------------------------
//// Function: CreateTransformationMatrix
//// Desc: 위치, 회전, 스케일 변환을 적용한 Gdiplus::Matrix를 생성합니다.
//// Params:
////   translateX - X축 이동 값
////   translateY - Y축 이동 값
////   rotateAngleDegrees - 회전 각도 (도 단위)
////   scaleX - X축 스케일 값
////   scaleY - Y축 스케일 값
//// Returns:
////   변환이 적용된 Gdiplus::Matrix 객체
////-----------------------------------------------------------------------------
//Gdiplus::Matrix CreateTransformationMatrix(float translateX, float translateY, float rotateAngleDegrees, float scaleX, float scaleY) {
//    // 1. 단위 행렬로 시작합니다.
//    Gdiplus::Matrix mat; // 기본 생성자는 단위 행렬(Identity Matrix)을 생성합니다.
//
//    // 2. 스케일 변환을 적용합니다. (항상 가장 먼저 적용하는 것이 좋습니다.)
//    // GDI+의 ApplyOrderPrepend는 새로운 변환을 기존 변환 "이전"에 적용합니다.
//    // 따라서, 연산 순서를 (스케일 -> 회전 -> 이동)으로 하고 싶다면,
//    // Add 함수들을 호출하는 순서는 (이동 -> 회전 -> 스케일)이 됩니다.
//    mat.Scale(scaleX, scaleY, Gdiplus::MatrixOrderPrepend);
//
//    // 3. 회전 변환을 적용합니다.
//    mat.Rotate(rotateAngleDegrees, Gdiplus::MatrixOrderPrepend);
//
//    // 4. 이동 변환을 적용합니다.
//    mat.Translate(translateX, translateY, Gdiplus::MatrixOrderPrepend);
//
//    return mat;
//}
