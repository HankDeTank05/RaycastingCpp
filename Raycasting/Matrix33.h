#ifndef MATRIX_22_H
#define MATRIX_22_H

#include "Vector3.h"

class Matrix33
{
public:
	Matrix33();
	Matrix33(const Matrix33& m) = delete;
	Matrix33& operator=(const Matrix33& m) = delete;
	virtual ~Matrix33();

public: // specialized constructors
	Matrix33(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	Matrix33(Vector3 c0, Vector3 c1, Vector3 c2);

public: // accessors
	Vector3 GetCol(int colNum);
	Vector3 GetRow(int rowNum);
	float Get(int rowNum, int colNum);

private: // internal helper functions
	void BoundsCheck(int index);

private: // member variables
	float m[3][3];

private: // prevent implicit conversions
	Matrix33(int, int, int,
		int, int, int,
		int, int, int);
	Matrix33(double, double, double,
		double, double, double,
		double, double, double);

};

#endif