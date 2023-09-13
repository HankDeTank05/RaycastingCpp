#ifndef MATRIX_44_H
#define MATRIX_44_H

#include "Vector4.h"

class Matrix44
{
public:
	Matrix44();
	Matrix44(const Matrix44& m);
	Matrix44& operator=(const Matrix44& m);
	virtual ~Matrix44();

	// specialized constructor
	Matrix44(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3);

	// other "constructors"
	static Matrix44 RotY(float angle);

public: // accessors
	float Get(int row, int col) const;
	Vector4 GetCol(int colNum) const;
	Vector4 GetRow(int rowNum) const;

public: // mutators
	void Set(int row, int col, float value);
	void SetCol(int colNum, Vector4 col);
	void SetRow(int rowNum, Vector4 row);

public: // operators
	Vector4 operator*(const Vector4& vector) const;
	Matrix44 operator*(float scalar) const;
	Matrix44 operator*(const Matrix44& other) const;

private: // internal helper functions
	void BoundsCheck(int index) const;

private:
	float m[4][4];
};

#endif