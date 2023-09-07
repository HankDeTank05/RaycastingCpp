#ifndef VECTOR_3_H
#define VECTOR_3_H

class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& v) = delete;
	Vector3& operator=(const Vector3& v) = delete;
	virtual ~Vector3();

	// specialized constructor
	Vector3(float x, float y, float z);

	// accessors

	float GetX();
	float GetY();
	float GetZ();

private:
	float x;
	float y;
	float z;

private:
	// prevent implicit conversions
	Vector3(int, int, int);
	Vector3(double, double, double);
};

#endif