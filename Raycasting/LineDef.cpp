#include "LineDef.h"

LineDef::~LineDef()
{
	// do nothing (for now)
}

LineDef::LineDef(float x0, float y0, float x1, float y1, float nx, float ny)
	: p0(x0, y0, 1.0f),
	p1(x1, y1, 1.0f),
	normal(nx, ny, 0.0f)
{
	// do nothing (for now)
}
