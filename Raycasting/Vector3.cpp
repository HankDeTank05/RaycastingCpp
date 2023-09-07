#include "Vector3.h"

Vector3::Vector3()
	: x(0.0f),
	y(0.0f),
	z(0.0f)
{
	// do nothing
}

Vector3::~Vector3()
{
	// do nothing
}

Vector3::Vector3(float _x, float _y, float _z)
	: x(_x),
	y(_y),
	z(_z)
{
	// do nothing
}

float Vector3::GetX()
{
	return x;
}

float Vector3::GetY()
{
	return y;
}

float Vector3::GetZ()
{
	return z;
}
