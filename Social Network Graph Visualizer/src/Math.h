#pragma once

namespace Math
{ 
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegInfinity = -std::numeric_limits<float>::infinity();

	float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	bool NearZero(float val)
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
}

template<typename T> class Vector2D
{
public:
	Vector2D() : x(0), y(0) { ; }
	explicit Vector2D(T x, T y) : x(x), y(y) { ; }

public:
	T x;
	T y;
};