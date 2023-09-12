#include "LineDef.h"

LineDef::LineDef(const LineDef& ld)
	: p0(ld.p0),
	p1(ld.p1),
	normal(ld.normal)
{
	// do nothing
}

LineDef::~LineDef()
{
	// do nothing (for now)
}

LineDef::LineDef(float x0, float z0, float x1, float z1)
	: p0(x0, 0.0f, z0, 1.0f),
	p1(x1, 0.0f, z1, 1.0f),
	normal((p1 - p0).cross(Vector4(0.0f, 1.0f, 0.0f, 0.0f)))
{
	// do nothing (for now)
}

Vector4 LineDef::GetP0() const
{
	return p0;
}

Vector4 LineDef::GetNormal() const
{
	return normal;
}
