#include "Matrix33.h"
#include <cassert>

Matrix33::Matrix33()
	: m()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
}

Matrix33::~Matrix33()
{
	// do nothing
}

Matrix33::Matrix33(float m00, float m01, float m02,
					float m10, float m11, float m12,
					float m20, float m21, float m22)
	: m()
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Matrix33::Matrix33(Vector3 c0, Vector3 c1, Vector3 c2)
	: m()
{
	m[0][0] = c0.GetX(); m[0][1] = c1.GetX(); m[0][2] = c2.GetX();
	m[1][0] = c0.GetY(); m[1][1] = c1.GetY(); m[1][2] = c2.GetY();
	m[2][0] = c0.GetZ(); m[2][1] = c1.GetZ(); m[2][2] = c2.GetZ();
}

Vector3 Matrix33::GetCol(int colNum)
{
	BoundsCheck(colNum);

	return Vector3(m[0][colNum], m[1][colNum], m[2][colNum]);
}

Vector3 Matrix33::GetRow(int rowNum)
{
	BoundsCheck(rowNum);

	return Vector3(m[rowNum][0], m[rowNum][1], m[rowNum][2]);
}

float Matrix33::Get(int rowNum, int colNum)
{
	BoundsCheck(rowNum);
	BoundsCheck(colNum);

	return m[rowNum][colNum];
}

void Matrix33::BoundsCheck(int index)
{
	assert(index >= 0);
	assert(index < 3);
}
