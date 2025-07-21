#pragma once
#include <windows.h>
#include <gdiplus.h>

#include "../Core/Vector.h"
#include <cmath>



using namespace Gdiplus;

class MyMath
{
public:
    MyMath() = default;
    virtual ~MyMath() = default;

public:
	static Gdiplus::Status GetBoundBox( const Gdiplus::Matrix* p_transform_mat
		, const Gdiplus::Matrix* p_camera_mat
		, const float& width
		, const float& height
		, OUT Gdiplus::RectF* p_rect)
	{
        // ✅ 1. 로컬 좌표에서의 4개 꼭짓점 정의
        PointF pts[4] = {
            PointF(0.0f, 0.0f), // Left-Top
            PointF(width, 0.0f), // Right-Top
            PointF(width, height), // Right-Bottom
            PointF(0.0f, height) // Left-Bottom
        };

        //// ✅ 2. 매트릭스를 이용해 변환
        //Matrix worldMatrix = *p_srcmat;
        //worldMatrix.TransformPoints(pts, 4);
        p_transform_mat->TransformPoints(pts, 4);

		if (p_camera_mat)
			p_camera_mat->TransformPoints(pts, 4);

        // ✅ 3. Bounding Box 계산
        float minX = pts[0].X, maxX = pts[0].X;
        float minY = pts[0].Y, maxY = pts[0].Y;

        for (int i = 1; i < 4; i++) {
            if (pts[i].X < minX) minX = pts[i].X;
            if (pts[i].X > maxX) maxX = pts[i].X;
            if (pts[i].Y < minY) minY = pts[i].Y;
            if (pts[i].Y > maxY) maxY = pts[i].Y;
        }


		p_rect->X = minX;
        p_rect->Y = minY;
        p_rect->Width = maxX - minX;
        p_rect->Height = maxY - minY;


		return Gdiplus::Status::Ok;
	}


    static Gdiplus::Status GetBoundBox(const Gdiplus::Matrix* p_srcmat
		, const Gdiplus::Matrix* p_camera_mat
		, const SizeF& size
		, OUT Gdiplus::RectF* p_rect)
	{
        // ✅ 1. 로컬 좌표에서의 4개 꼭짓점 정의
        PointF pts[4] = {
            PointF(0.0f, 0.0f), // Left-Top
            PointF(size.Width, 0.0f), // Right-Top
            PointF(size.Width, size.Height), // Right-Bottom
            PointF(0.0f, size.Height) // Left-Bottom
        };

        //// ✅ 2. 매트릭스를 이용해 변환
        //Matrix worldMatrix = *p_srcmat;
        //worldMatrix.TransformPoints(pts, 4);
		p_srcmat->TransformPoints(pts, 4);
        if (p_camera_mat) 
			p_camera_mat->TransformPoints(pts, 4);

        // ✅ 3. Bounding Box 계산
        float minX = pts[0].X, maxX = pts[0].X;
        float minY = pts[0].Y, maxY = pts[0].Y;

        for (int i = 1; i < 4; i++) {
            if (pts[i].X < minX) minX = pts[i].X;
            if (pts[i].X > maxX) maxX = pts[i].X;
            if (pts[i].Y < minY) minY = pts[i].Y;
            if (pts[i].Y > maxY) maxY = pts[i].Y;
        }

		p_rect->X = minX;
        p_rect->Y = minY;
        p_rect->Width = maxX - minX;
        p_rect->Height = maxY - minY;

		return Gdiplus::Status::Ok;
	}

    static Gdiplus::Status GetMatrix(OUT Gdiplus::Matrix* p_srcmat
		, const Vec2& pos
		, const Gdiplus::SizeF& scale
		, float rotation) {

        p_srcmat->Translate(pos.x, pos.y);
        p_srcmat->Rotate(rotation);
        return p_srcmat->Scale(scale.Width, scale.Height);
    }

	static Gdiplus::Status SetRect(OUT Gdiplus::RectF* p_rect
		, const Vec2& pos
		, const Gdiplus::SizeF& size) {
		p_rect->X = pos.x;
		p_rect->Y = pos.y;
		p_rect->Width = size.Width;
		p_rect->Height = size.Height;
		return Gdiplus::Ok;
    }

	static Gdiplus::Status SetRect( OUT Gdiplus::RectF* p_rect, const Vec2& p_min, const Vec2& p_max ) {
        p_rect->Width = p_max.x - p_min.x;
        p_rect->Height = p_max.y - p_min.y;
        p_rect->X = p_min.x + (p_rect->Width * 0.5f);
        p_rect->Y = p_min.y + (p_rect->Height * 0.5f);
        return Gdiplus::Ok;
    }

protected:
	static float DegToRad(float deg) {
		return deg * 3.14159265358979323846 / 180.0f;
	}
	static float RadToDeg(float rad) {
		return rad * 180.0f / 3.14159265358979323846; 
	}

};

