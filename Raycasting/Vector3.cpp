#include "Vector3.h"
#include <cassert>

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

Vector3 Vector3::operator+(const Vector3& other)
{
	assert(z == 0.0f || other.z == 0.0f);
	return Vector3(x + other.x, y + other.y, z);
}

Vector3 Vector3::operator+=(const Vector3& other)
{
	return (*this) + other;
}

Vector3 Vector3::operator-(const Vector3& other)
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator-=(const Vector3& other)
{
	return (*this) - other;
}

Vector3 Vector3::operator*(float scalar)
{
	return Vector3(x * scalar, y * scalar, z);
}

Vector3 Vector3::operator*=(float scalar)
{
	return (*this) * scalar;
}

Vector3 Vector3::operator*(int scalar)
{
	return (*this) * static_cast<float>(scalar);
}

Vector3 Vector3::operator*=(int scalar)
{
	return (*this) * scalar;
}

Vector3 Vector3::operator/(float scalar)
{
	assert(scalar > 0.0f || scalar < 0.0f);
	return Vector3(x / scalar, y / scalar, z);
}

Vector3 Vector3::operator/(int scalar)
{
	return (*this) / static_cast<float>(scalar);
}

Vector3 Vector3::operator*(double scalar)
{
	return Vector3();
}

Vector3 Vector3::operator/(double scalar)
{
	return Vector3();
}

float Vector3::GetX() const
{
	return x;
}

float Vector3::GetY() const
{
	return y;
}

float Vector3::GetZ() const
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

Vector3 operator*(float scalar, const Vector3& vector)
{
	return Vector3(scalar * vector.x, scalar * vector.y, vector.z);
}

Vector3 operator*(int scalar, const Vector3& vector)
{
	return static_cast<float>(scalar) * vector;
}

Vector3 operator*(double scalar, const Vector3& vector)
{
	return Vector3();
}
