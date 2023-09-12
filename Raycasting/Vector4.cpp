#include "Vector4.h"
#include <cassert>
#include <corecrt_math.h>

Vector4::Vector4()
	: x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{
	// do nothing
}

Vector4::Vector4(const Vector4& v)
	: x(v.x),
	y(v.y),
	z(v.z),
	w(v.w)
{
	// do nothing
}

Vector4& Vector4::operator=(const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return *this;
}

Vector4::~Vector4()
{
	// do nothing
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
	: x(_x),
	y(_y),
	z(_z),
	w(_w)
{
	// do nothing
}

float Vector4::mag() const
{
	return sqrtf(this->magSqr());
}

float Vector4::magSqr() const
{
	return this->dot(*this);
}

float Vector4::dot(const Vector4& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector4 Vector4::cross(const Vector4& other) const
{
	assert(this->w == 0.0f);
	assert(other.w == 0.0f);

	// u = (x, y, z, 0.0f)
	// v = (x, y, z, 0.0f)

	// ux    uy    uz    uw
	//    \/ \/ \/
	//    /\ /\ /\
	// vx    vy    vz    vw
	//
	// (u x v).x = uy * vz - uz * vy
	// (u x v).y = uz * vx - ux * vz
	// (u x v).z = ux * vy - uy * vx

	return Vector4(this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x,
		0.0f);
}

Vector4 Vector4::operator+(const Vector4& other)
{
	// p = point = (?, ?, ?, 1.0f)
	// v = vector = (?, ?, ?, 0.0f)

	// p + p = NOPE
	// p + v = OKAY
	// v + p = OKAY
	// v + v = OKAY

	assert(this->w + other.w <= 1.0f);
	return Vector4(this->x + other.x,
		this->y + other.y,
		this->z + other.z,
		this->w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other)
{
	return Vector4(this->x - other.x,
		this->y - other.y,
		this->z - other.z,
		this->w - other.w);
}

Vector4 Vector4::operator*(float scalar)
{
	return Vector4(x * scalar, y * scalar, z * scalar, w);
}

bool Vector4::operator==(const Vector4& other) const
{
	return this->x == other.x &&
		this->y == other.y &&
		this->z == other.z &&
		this->w == other.w;
}

float Vector4::GetX() const
{
	return x;
}

float Vector4::GetY() const
{
	return y;
}

float Vector4::GetZ() const
{
	return z;
}

float Vector4::GetW() const
{
	return w;
}

void Vector4::SetX(float newX)
{
	x = newX;
}

void Vector4::SetY(float newY)
{
	y = newY;
}

void Vector4::SetZ(float newZ)
{
	z = newZ;
}

void Vector4::SetW(float newW)
{
	w = newW;
}

Vector4 operator*(float scalar, const Vector4& vector)
{
	return Vector4(scalar * vector.x, scalar * vector.y, scalar * vector.z, vector.w);
}
