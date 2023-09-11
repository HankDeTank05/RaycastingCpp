#ifndef VECTOR_4_H
#define VECTOR_4_H

class Vector4
{
public:
	Vector4();
	Vector4(const Vector4& v4) = delete;
	Vector4& operator=(const Vector4& v4) = delete;
	virtual ~Vector4();
};

#endif