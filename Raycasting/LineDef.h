#ifndef LINE_DEF_H
#define LINE_DEF_H

#include "Vector4.h"

class LineDef
{
public:
	LineDef() = delete;
	LineDef(const LineDef& ld);
	LineDef& operator=(const LineDef& ld) = delete;
	virtual ~LineDef();

	LineDef(float x0, float z0, float x1, float z1);

	Vector4 GetP0() const;
	Vector4 GetP1() const;
	Vector4 GetNormal() const;
	
protected:
	Vector4 p0;
	Vector4 p1;
	Vector4 normal;
};

#endif