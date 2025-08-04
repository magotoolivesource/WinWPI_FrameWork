#pragma once
#include <cmath>

struct Vec2 {

public:
	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2& other)
        : x(other.x)
        , y(other.y) { }

public:
    float x, y;

    Vec2 Rotate(float angleDegrees) const {
        float rad = angleDegrees * 3.14159265f / 180.0f;
        float cosA = cosf(rad);
        float sinA = sinf(rad);
        return {
            x * cosA - y * sinA,
            x * sinA + y * cosA
        };
    }

	void Reset( ) { x = 0; y = 0; }
	void Set(float p_x, float p_y) { x = p_x; y = p_y; }

#pragma region 코파일럿 자동 생성자들

	Vec2 operator+(const Vec2& other) const {
		return { x + other.x, y + other.y };
	}
	Vec2 operator-(const Vec2& other) const {
		return { x - other.x, y - other.y };
	}
	Vec2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}
	Vec2 operator/(float scalar) const {
		return { x / scalar, y / scalar };
	}
	bool operator==(const Vec2& other) const {
		return x == other.x && y == other.y;
	}
	Vec2 operator-() const {
		return { -x, -y };
	}

    float Dot(const Vec2& other) const 
	{ 
		return x * other.x + y * other.y; 
	}
	float Length() const {
		return sqrtf(x * x + y * y);
	}
	float LengthSquared() const {
		return x * x + y * y;
	}
	Vec2 Normalize() const {
		float len = Length();
		if (len == 0) return { 0, 0 };
		return { x / len, y / len };


		//float lenSq = x * x + y * y;
  //      if (lenSq == 0.0f) return { 0, 0 };

  //      // Quake III fast inverse square root
  //      float x2 = lenSq * 0.5f;
  //      float y = lenSq;
  //      long i = *(long*)&y;
  //      i = 0x5f3759df - (i >> 1);
  //      y = *(float*)&i;
  //      y = y * (1.5f - (x2 * y * y)); // 1회 iteration

  //      return { x * y, y * y };
	}
	Vec2 Perpendicular() const {
		return { -y, x };
	}
	Vec2 Lerp(const Vec2& other, float t) const {
		return { x + (other.x - x) * t, y + (other.y - y) * t };
	}
	Vec2 Clamp(const Vec2& min, const Vec2& max) const {
		return {
			x < min.x ? min.x : (x > max.x ? max.x : x),
			y < min.y ? min.y : (y > max.y ? max.y : y)
		};
	}
	
	Vec2 Reflect(const Vec2& normal) const {
		float dot = x * normal.x + y * normal.y;
		return { x - 2 * dot * normal.x, y - 2 * dot * normal.y };
	}
	Vec2 Project(const Vec2& other) const {
		float dot = (x * other.x + y * other.y) / (other.x * other.x + other.y * other.y);
		return { dot * other.x, dot * other.y };
	}
	Vec2 Abs() const {
		return { fabsf(x), fabsf(y) };
	}
	//Vec2 Sign() const {
	//	return { (this->x > 0.f) - ( this->x < 0.f), ( this->y > 0.f) - ( this->y < 0.f) };
	//}
	Vec2 Floor() const {
		return { floorf(x), floorf(y) };
	}
	Vec2 Ceil() const {
		return { ceilf(x), ceilf(y) };
	}
	Vec2 Round() const {
		return { roundf(x), roundf(y) };
	}
	Vec2 Min(const Vec2& other) const {
		return { x < other.x ? x : other.x, y < other.y ? y : other.y };
	}
	Vec2 Max(const Vec2& other) const {
		return { x > other.x ? x : other.x, y > other.y ? y : other.y };
	}
	Vec2 operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2 operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2 operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	Vec2 operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}
	Vec2 operator*(const Vec2& other) const {
		return { x * other.x, y * other.y };
	}
	Vec2 operator/(const Vec2& other) const {
		return { x / other.x, y / other.y };
	}
	Vec2 operator+=(float scalar) {
		x += scalar;
		y += scalar;
		return *this;
	}

#pragma endregion

};