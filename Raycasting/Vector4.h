#ifndef VECTOR_4_H
#define VECTOR_4_H

class Vector4
{
public:
	Vector4();
	Vector4(const Vector4& v);
	Vector4& operator=(const Vector4& v);
	virtual ~Vector4();

	// specialized constructor
	Vector4(float x, float y, float z, float w);

public: // vector math
	float mag() const;
	float magSqr() const;
	float dot(const Vector4& other) const;
	Vector4 cross(const Vector4& other) const;

public: // operators
	Vector4 operator+(const Vector4& other);

	Vector4 operator-(const Vector4& other);

	Vector4 operator*(float scalar);
	friend Vector4 operator*(float scalar, const Vector4& vector);

	bool operator==(const Vector4& other) const;

public: // accessors
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

public: // mutators
	void SetX(float newX);
	void SetY(float newY);
	void SetZ(float newZ);
	void SetW(float newW);

private:
	float x;
	float y;
	float z;
	float w;
};

#endif