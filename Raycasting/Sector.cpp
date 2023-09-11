#include "Sector.h"

Sector::Sector()
	: walls()
{
	// do nothing (for now)
}

Sector::~Sector()
{
	// do nothing (for now)
}

void Sector::AddWall(int x0, int y0, int x1, int y1, float nx, float ny)
{
	walls.push_back({ x0, y0, x1, y1, Vector3(nx, ny, 0.0f)});
}
