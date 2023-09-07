#include "Vector3.h"

Vector3::Vector3()
	: x(0.0f),
	y(0.0f),
	z(0.0f)
{
	// do nothing
}

Vector3::Vector3(const Vector3& v)
	: x(v.x),
	y(v.y),
	z(v.z)
{
	// do nothing
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
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

void Vector3::SetX(float newX)
{
	this->x = newX;
}

void Vector3::SetY(float newY)
{
	this->y = newY;
}

void Vector3::SetZ(float newZ)
{
	this->z = newZ;
}

void Vector3::Set(float newX, float newY, float newZ)
{
	this->SetX(newX);
	this->SetY(newY);
	this->SetZ(newZ);
}
