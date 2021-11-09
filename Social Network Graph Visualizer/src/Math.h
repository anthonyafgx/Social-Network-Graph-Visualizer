#pragma once

// integer version
// TO DO: Implement Inheritance
struct Coordinates
{
public:
	Coordinates() : x(0), y(0) { ; }
	explicit Coordinates(int x, int y) : x(x), y(y) { ; }

public:
	int x;
	int y;
};

struct Vector2D
{
public:
	Vector2D() : x(0.0f), y(0.0f) { ; }
	explicit Vector2D(float x, float y) : x(x), y(y) { ; }

public:
	float x;
	float y;
};

