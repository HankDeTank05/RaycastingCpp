#ifndef VECTOR_3_H
#define VECTOR_3_H

class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& v);
	Vector3& operator=(const Vector3& v);
	virtual ~Vector3();

public: // specialized constructor
	Vector3(float x, float y, float z);

public: // operators
	Vector3 operator+(const Vector3& other);
	Vector3 operator+=(const Vector3& other);

	Vector3 operator-(const Vector3& other);
	Vector3 operator-=(const Vector3& other);

	Vector3 operator*(float scalar);
	Vector3 operator*=(float scalar);
	Vector3 operator*(int scalar);
	Vector3 operator*=(int scalar);
	friend Vector3 operator*(float scalar, const Vector3& vector);
	friend Vector3 operator*(int scalar, const Vector3& vector);

	Vector3 operator/(float scalar);
	Vector3 operator/(int scalar);

private: // prevent implicit conversions
	Vector3 operator*(double scalar);
	friend Vector3 operator*(double scalar, const Vector3& vector);

	Vector3 operator/(double scalar);

public: // accessors
	float GetX() const;
	float GetY() const;
	float GetZ() const;

public: // mutators
	void SetX(float newX);
	void SetY(float newY);
	void SetZ(float newZ);
	void Set(float newX, float newY, float newZ);

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