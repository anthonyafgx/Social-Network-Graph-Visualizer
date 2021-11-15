#pragma once
#include <limits>

namespace Math
{ 
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	inline bool NearZero(float val)
	{
		float epsilon = 0.001f;		// small value
		
		if (fabs(val) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline int Random(int from, int to)
	{
		return (rand() % (to - from + 1)) + from;
	}

	inline bool IsPowerOfTwo(int num)
	{
		return (num != 0) && ((num & (num - 1)) == 0);
	}
}

template<typename T> class Vector2D
{
public:
	Vector2D() : x(0), y(0) { ; }
	explicit Vector2D(T x, T y) : x(x), y(y) { ; }

	friend Vector2D<T> operator+ (const Vector2D<T>& a, const Vector2D<T>& b)
	{
		return Vector2D<T>(a.x + b.x, a.y + b.y);
	}

	friend Vector2D<T> operator- (const Vector2D<T>& a, const Vector2D<T>& b)
	{
		return Vector2D<T>(a.x - b.x, a.y - b.y);
	}

	friend Vector2D<T> operator* (const Vector2D<T>& vec, const T scalar)
	{
		return Vector2D<T>(vec.x * scalar, vec.y * scalar);
	}

	Vector2D<T>& operator+= (const Vector2D<T>& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2D<T>& operator-= (const Vector2D<T>& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

public:
	T x;
	T y;
};