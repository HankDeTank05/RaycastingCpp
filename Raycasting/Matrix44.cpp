#include "Matrix44.h"

#include <corecrt_math.h>
#include <cassert>

Matrix44::Matrix44()
	: m()
{
	// identity matrix
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

Matrix44::Matrix44(const Matrix44& m)
	: m()
{
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			this->m[row][col] = m.m[row][col];
		}
	}
}

Matrix44& Matrix44::operator=(const Matrix44& m)
{
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			this->m[row][col] = m.m[row][col];
		}
	}

	return *this;
}

Matrix44::~Matrix44()
{
	// do nothing
}

Matrix44::Matrix44(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
	: m()
{
	m[0][0] = c0.GetX(); m[0][1] = c1.GetX(); m[0][2] = c2.GetX(); m[0][3] = c3.GetX();
	m[1][0] = c0.GetY(); m[1][1] = c1.GetY(); m[1][2] = c2.GetY(); m[1][3] = c3.GetY();
	m[2][0] = c0.GetZ(); m[2][1] = c1.GetZ(); m[2][2] = c2.GetZ(); m[2][3] = c3.GetZ();
	m[3][0] = c0.GetW(); m[3][1] = c1.GetW(); m[3][2] = c2.GetW(); m[3][3] = c3.GetW();
}

Matrix44 Matrix44::RotY(float angle)
{
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	Vector4 c0, c1, c2, c3;

	/*
	* rotation matrix for rotation about y axis
	* 
	* [	cos(angle)	0.0f	sin(angle)	0.0f	]
	* [		0.0f	1.0f		0.0f	0.0f	]
	* [	-sin(angle)	0.0f	cos(angle)	0.0f	]
	* [		0.0f	0.0f		0.0f	1.0f	]
	*/

	c0.SetX(+cosAngle);/*     0.0f     */c2.SetX(+sinAngle);/*     0.0f     */
	/*      0.0f       */c1.SetY(1.0f);/*       0.0f               0.0f     */
	c0.SetZ(-sinAngle);/*     0.0f     */c2.SetZ(+cosAngle);/*     0.0f     */
	/*      0.0f              0.0f              0.0f        */c2.SetW(1.0f);

	return Matrix44(c0, c1, c2, c3);
}

float Matrix44::Get(int row, int col) const
{
	BoundsCheck(row);
	BoundsCheck(col);

	return m[row][col];
}

Vector4 Matrix44::GetCol(int colNum) const
{
	BoundsCheck(colNum);

	return Vector4(m[0][colNum], m[1][colNum], m[2][colNum], m[3][colNum]);
}

Vector4 Matrix44::GetRow(int rowNum) const
{
	BoundsCheck(rowNum);

	return Vector4(m[rowNum][0], m[rowNum][1], m[rowNum][2], m[rowNum][3]);
}

void Matrix44::Set(int row, int col, float value)
{
	BoundsCheck(row);
	BoundsCheck(col);

	m[row][col] = value;
}

void Matrix44::SetCol(int colNum, Vector4 col)
{
	BoundsCheck(colNum);

	m[0][colNum] = col.GetX();
	m[1][colNum] = col.GetY();
	m[2][colNum] = col.GetZ();
	m[3][colNum] = col.GetW();
}

void Matrix44::SetRow(int rowNum, Vector4 row)
{
	BoundsCheck(rowNum);

	m[rowNum][0] = row.GetX(); m[rowNum][1] = row.GetY(); m[rowNum][2] = row.GetZ(); m[rowNum][3] = row.GetW();
}

Vector4 Matrix44::operator*(const Vector4& vector) const
{
	return Vector4(GetRow(0).dot(vector), GetRow(1).dot(vector), GetRow(2).dot(vector), GetRow(3).dot(vector));
}

Matrix44 Matrix44::operator*(float scalar) const
{
	Matrix44 multiplied = *this;

	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			multiplied.m[row][col] *= scalar;
		}
	}

	return multiplied;
}

Matrix44 Matrix44::operator*(const Matrix44& other) const
{
	return Matrix44();
}

void Matrix44::BoundsCheck(int index) const
{
	assert(0 <= index);
	assert(index < 4);
}
