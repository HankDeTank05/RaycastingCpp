#ifndef LINE_DEF_H
#define LINE_DEF_H

#include "Vector3.h"

class LineDef
{
public:
	LineDef() = delete;
	LineDef(const LineDef& ld) = delete;
	LineDef& operator=(const LineDef& ld) = delete;
	virtual ~LineDef();

	LineDef(float x0, float y0, float x1, float y1, float nx, float ny);

protected:
	Vector3 p0;
	Vector3 p1;
	Vector3 normal;
};

#endif